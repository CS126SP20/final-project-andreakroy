// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

#include <cinder/app/App.h>
#include <cinder/gl/draw.h>

#include "chess/board.h"
#include "chess/game.h"
#include <curl/curl.h>
#include "cinder/ImageIo.h"
#include "cinder/audio/audio.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/gl.h"
#include <json.hpp>

namespace myapp {

using board::kSquareSize;
using cinder::Rectf;
using cinder::app::MouseEvent;
using cinder::gl::Texture;
using nlohmann::json;

ci::audio::VoiceRef err_sound;
MyApp::MyApp() : game_(game::Game(3)) {
  player_ = game_.white_;
  turn_ = game_.white_;
}

void MyApp::setup() {
  ResetMoves();
  cinder::audio::SourceFileRef err = cinder::audio::load(
        cinder::app::loadAsset("err.mp3"));
    err_sound = cinder::audio::Voice::create(err);

}

void MyApp::update() {
  GetUpdate();
  game::Player* p = game_.white_;
  piece::Color prev_move = piece::Color::kBlack;
  if (turn_ == game_.black_) {
    p = game_.black_;
    prev_move = piece::Color::kWhite;
  }
  // If the destination and origin squares are not selected do nothing
  if (!destination_square_ || !origin_square_) {
    return;
  }
  // If the player selected the wrong color, do nothing
  if (origin_square_->piece_->color_ != p->color_) {
    return;
  }

  // If playing the move is successful
  auto m = p->PlayMove(origin_square_, destination_square_,
                                 &game_);
  if (game_.PlayTurn(m)) {
    if (p == game_.white_) {
      turn_ = game_.black_;
    } else {
      turn_ = game_.white_;
    }
  } else {
    err_sound->start();
  }
  ResetMoves();
}

void MyApp::draw() {
  DrawBoard();
}

void MyApp::mouseDown(MouseEvent event) {
  const board::Square* at = game_.board_->At(floor(event.getX() / kSquareSize),
                                      floor(event.getY() / kSquareSize));
  if (origin_square_) {
    if (at->piece_ && at->piece_->color_ == turn_->color_) {
      origin_square_ = at;
      destination_square_ = nullptr;
      return;
    }
    destination_square_ = at;
  } else {
    if (!at->piece_) {
      ResetMoves();
      return;
    }
    if (at->piece_->color_ == turn_->color_) {
      origin_square_ = at;
      destination_square_ = nullptr;
      return;
    }
    ResetMoves();
  }
  //TODO: Reset origin and destination ptr after a legal/illegal move
  //TODO: Show green if a legal move was played, red otherwise.
}

void MyApp::DrawBoard() {
  cinder::gl::clear();
  for (size_t j = 0; j < board::kSize; j++) {
    for (size_t i = 0; i < board::kSize; i++) {
      const board::Square* s = game_.board_->At(i, j);
      // When the square is not selected, set it to the appropriate color.
      cinder::gl::color(s->sq_color_);
      if (origin_square_ == s && !destination_square_) {
        // If the square is selected, highlight it yellow.
        cinder::gl::color(board::yellow);
      }
      cinder::gl::drawSolidRect(s->loc_);

      // Render pieces
      if (s->piece_ == nullptr) {
        continue;
      }
      cinder::gl::Texture2dRef ref = cinder::gl::Texture2d::create(
          cinder::loadImage(loadAsset(s->piece_->img_path_)));
      cinder::gl::draw(ref, s->loc_);
    }
  }
}

void MyApp::ResetMoves() {
  origin_square_ = nullptr;
  destination_square_ = nullptr;
}

size_t WriteCallBack(void *contents, size_t size, size_t nmemb, void *userp) {
  ((std::string*)userp)->append((char*)contents, size*nmemb);
  return size * nmemb;
}

void MyApp::GetUpdate() {
  std::string base_url = "http://127.0.0.1:5000/moves";
  std::string buffer;
  CURLcode res;
  CURL *curl;
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);
    curl_easy_setopt(curl, CURLOPT_URL,
                     (base_url + "?game_id=" + std::to_string(game_.id_)).c_str
                     ());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallBack);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      std::cout << "Could not find game with id: " + std::to_string(game_.id_)
                       + " on the server.";
      std::exit(-1);
    }
    curl_easy_cleanup(curl);
    std::cout << buffer;
    game::Move to_return;
    to_return.player_ = game_.white_;
    if (player_ == game_.white_) {
      to_return.player_ = game_.black_;
    }
    std::string move = json::parse(buffer)["move"];
    std::cout << move;
    to_return.from_ = game_.board_->At(move.at(0) - '0',
                                       move.at(1) - '0');

    to_return.to_ = game_.board_->At(move.at(2) - '0',
                                       move.at(3) - '0');
    to_return.state_ = game::MoveState::kSuccess;
    to_return.IsCastling_ = false;
    game_.moves_.emplace_back(to_return);
    return;
  }
  std::cout << "Could not connect to server.";
  std::exit(-1);
}
}  // namespace myapp

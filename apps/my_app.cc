// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

#include <cinder/app/App.h>
#include <cinder/gl/draw.h>
#include <curl/curl.h>

#include <json.hpp>
#include <stdio.h>
#include "chess/board.h"
#include "chess/game.h"
#include "cinder/ImageIo.h"
#include "cinder/audio/audio.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/gl.h"

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
  cinder::audio::SourceFileRef err =
      cinder::audio::load(cinder::app::loadAsset("err.mp3"));
  err_sound = cinder::audio::Voice::create(err);
}

void MyApp::update() {
  GetUpdate();
  game::Player *p = game_.white_;
  if (turn_ == game_.black_) {
    p = game_.black_;
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
  auto m = p->PlayMove(origin_square_, destination_square_, &game_);
  if (game_.PlayTurn(m)) {
    if (p == game_.white_) {
      turn_ = game_.black_;
    } else {
      turn_ = game_.white_;
    }
    PostUpdate(m);
  } else {
    err_sound->start();
  }
  ResetMoves();
}

void MyApp::draw() { DrawBoard(); }

void MyApp::mouseDown(MouseEvent event) {
  const board::Square *at = game_.board_->At(floor(event.getX() / kSquareSize),
                                             floor(event.getY() / kSquareSize));
  // If there is a registered origin square.
  if (origin_square_) {
    // If the square clicked is of the same color as the player's turn,
    // switch the origin square to the new square.
    if (at->piece_ && at->piece_->color_ == turn_->color_) {
      origin_square_ = at;
      destination_square_ = nullptr;
      return;
    }
    // Otherwise, the destination square is the new square.
    destination_square_ = at;
    return;
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
  }
  // TODO: Reset origin and destination ptr after a legal/illegal move
  // TODO: Show green if a legal move was played, red otherwise.
}

void MyApp::DrawBoard() {
  cinder::gl::clear();
  for (size_t j = 0; j < board::kSize; j++) {
    for (size_t i = 0; i < board::kSize; i++) {
      const board::Square *s = game_.board_->At(i, j);
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
  ((std::string *)userp)->append((char *)contents, size * nmemb);
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
    curl_easy_setopt(
        curl, CURLOPT_URL,
        (base_url + "?game_id=" + std::to_string(game_.id_)).c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallBack);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      std::cout << "Server Error" + std::to_string(res);
      std::exit(-1);
    }
    long response_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    if (response_code != request_ok) {
      return;
    }
    curl_easy_cleanup(curl);
    game::Move to_play;
    std::string move;
    try {
      move = json::parse(buffer)["move"];
    } catch (nlohmann::json::parse_error e) {
      std::cout << "parse error\n";
      return;
    }

    if (move == last_move_str) {
      return;
    }
    to_play = game_.GetMoveFromStr(move, player_);
    if (to_play.player_->color_ == piece::Color::kWhite) {
      turn_ = game_.black_;
    } else {
      turn_ = game_.black_;
    }
    game_.PlayTurn(to_play);
    last_move_str = move;
    return;
  }
  std::cout << "Could not connect to server.";
  std::exit(-1);
}

void MyApp::PostUpdate(const game::Move move) {
  assert(move.state_ == game::MoveState::kSuccess);
  std::string base_url = "http://127.0.0.1:5000/moves";
  std::cout << base_url;
  std::stringstream move_stream;
  move_stream << move;
  CURLcode res;
  CURL *curl;
  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  if (curl) {
    std::string post_fields =
        ("?number=" + std::to_string(move.number_) +
        "&color=" + piece::color_str_map.at(move.player_->color_) +
        "&move=" + move_stream.str() + "&game_id=" + std::to_string(game_
                                                                         .id_));
    curl_easy_setopt(curl, CURLOPT_URL, (base_url + post_fields).c_str());
    //curl_easy_setopt(curl, CURLOPT_HTTPHEADER, "Transfer-Encoding: chunked");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "number=andrea");
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    //std::exit(-1);
  }
  curl_global_cleanup();
  //std::exit(0);
    /*if (res != CURLE_OK) {
      std::cout << "Could not post move to server in game with id: " +
                       std::to_string(game_.id_) + ".";
      std::exit(-1);
    }
    long response_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    if (response_code != request_ok) {
      std::cout << "bad request";
      return;
    }
    last_move_str = move_stream.str();
    curl_easy_cleanup(curl);
    return;
  }

  std::cout << "Could not connect to server.";
  std::exit(-1);*/
}
}  // namespace myapp


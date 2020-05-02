// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

#include <cinder/app/App.h>
#include <cinder/gl/draw.h>
#include <curl/curl.h>
#include <gflags/gflags.h>
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

DECLARE_uint32(game_id);
DECLARE_string(color);
DECLARE_string(url);

ci::audio::VoiceRef err_sound;

MyApp::MyApp() : game_(game::Game(FLAGS_game_id)) {
  if (FLAGS_color == "black") {
    player_ = game_.black_;
  } else if (FLAGS_color == "white") {
    player_ = game_.white_;
  } else {
    player_ = nullptr;
  }
  turn_ = game_.white_;
  url_ = FLAGS_url;
  if (url_.empty()) {
    player_ = nullptr;
  }
}

void MyApp::setup() {
  ResetMoves();
  cinder::audio::SourceFileRef err =
      cinder::audio::load(cinder::app::loadAsset("err.mp3"));
  err_sound = cinder::audio::Voice::create(err);
}

void MyApp::update() {
  if (player_ != nullptr && !url_.empty()) {
    GetUpdate();
  }

  if (player_ != turn_ && player_ != nullptr) {
    return;
  }

  // If the destination and origin squares are not selected do nothing
  if (!destination_square_ || !origin_square_) {
    return;
  }
  // If the player selected the wrong color, do nothing
  if (origin_square_->IsEmpty() || origin_square_->piece_->color_ !=
      turn_->color_) {
    return;
  }

  // If playing the move is successful
  auto m = turn_->PlayMove(origin_square_, destination_square_, &game_);
  if (game_.PlayTurn(m)) {
    if (turn_ == game_.white_) {
      turn_ = game_.black_;
    } else {
      turn_ = game_.white_;
    }
    if (player_ != nullptr && !url_.empty()) {
      PostUpdate(m);
    }
    switch(game_.EvaluateBoard()) {
      case game::GameState::kIP:
        std::cout << "progress";
        return;
      case game::GameState::kWhiteWin:
        std::cout << "player1";
        return;
      case game::GameState::kBlackWin:
        std::cout << "player2";
        return;
      case game::GameState::kDraw:
        std::cout << "draw";
        return;
    }
  } else {
    err_sound->start();
  }
  ResetMoves();
}

void MyApp::draw() { DrawBoard(); }

void MyApp::mouseDown(MouseEvent event) {
  const board::Square *at = game_.board_->At(floor(event.getX() / kSquareSize),
                                             floor(event.getY() / kSquareSize));
  if (turn_ != player_ && player_ != nullptr) {
    return;
  }
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

// CURL callback function for get request.
size_t WriteCallBack(void *contents, size_t size, size_t nmemb, void *userp) {
  ((std::string *)userp)->append((char *)contents, size * nmemb);
  return size * nmemb;
}

void MyApp::GetUpdate() {
  std::string buffer;
  CURLcode res;
  CURL *curl;
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);
    curl_easy_setopt(
        curl, CURLOPT_URL,
        (url_ + "?game_id=" + std::to_string(game_.id_)).c_str());
    curl_easy_setopt(curl, CURLOPT_URL,
                     (url_ + "?game_id=" + std::to_string(game_.id_)).c_str
                     ());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallBack);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      std::cout << "Server Error" + std::to_string(res);
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
    std::stringstream last;
    last << last_move_;
    if (move == last.str()) {
      return;
    }
    game::Player* prev_move = game_.white_;
    if (player_ == game_.white_) {
      prev_move = game_.black_;
    }
    to_play = game_.GetMoveFromStr(move, prev_move);
    if (prev_move->color_ == piece::Color::kWhite) {
      turn_ = game_.black_;
    } else {
      turn_ = game_.white_;
    }
    game_.PlayTurn(to_play);
    last_move_ = to_play;
    game::Move to_return;
    to_return.player_ = game_.white_;
    if (player_ == game_.white_) {
      to_return.player_ = game_.black_;
    }
    return;
  }
  std::cout << "Could not connect to server.";
  std::exit(-1);
}

void MyApp::PostUpdate(const game::Move move) {
  std::stringstream move_stream;
  move_stream << move;
  CURLcode res;
  CURL *curl;
  curl = curl_easy_init();
  if (curl) {
    std::string post_fields =
        ("?number=" + std::to_string(move.number_) +
        "&color=" + piece::color_str_map.at(move.player_->color_) +
        "&move=" + move_stream.str() + "&game_id=" + std::to_string(game_
                                                                         .id_));
    curl_easy_setopt(curl, CURLOPT_URL, (url_ + post_fields).c_str());
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");
    curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0);
    res = curl_easy_perform(curl);
    if (res != request_ok) {
      std::cout << "Error connecting to server.";
    }
    curl_easy_cleanup(curl);
    last_move_ = move;
  }
}
}  // namespace myapp


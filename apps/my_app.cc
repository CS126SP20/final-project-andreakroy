// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

#include <cinder/app/App.h>
#include <cinder/gl/draw.h>

#include "../include/chess/board.h"
#include "../include/chess/game.h"
#include <curl/curl.h>
#include "cinder/ImageIo.h"
#include "cinder/audio/audio.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/gl.h"

namespace myapp {

using board::kSquareSize;
using cinder::Rectf;
using cinder::app::MouseEvent;
using cinder::gl::Texture;

ci::audio::VoiceRef err_sound;
MyApp::MyApp() :
      game_(game::Game(3)) {
}

void MyApp::setup() {
  ResetMoves();
  turn_ = game_.white_;
  cinder::audio::SourceFileRef err = cinder::audio::load(
        cinder::app::loadAsset("err.mp3"));
    err_sound = cinder::audio::Voice::create(err);

  std::string buffer;
  CURLcode res;
  CURL *curl;
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "https://www.google.com");
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      std::cout << "err";
    }
    curl_easy_cleanup(curl);
    std::cout << buffer;
  }
}

void MyApp::update() {
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
}  // namespace myapp

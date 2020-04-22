// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

#include <chess/board.h>
#include <cinder/app/App.h>
#include <cinder/gl/draw.h>

#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/gl.h"

namespace myapp {

using board::kSquareSize;
using cinder::Rectf;
using cinder::app::MouseEvent;
using cinder::gl::Texture;

MyApp::MyApp() {}

void MyApp::setup() {
  ResetMoves();
  turn_ = game_.p1_;
}

void MyApp::update() {
  game::Player* p = game_.p1_;
  if (turn_ == game_.p2_) {
    p = game_.p2_;
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
  if (game_.PlayTurn(p->PlayMove(origin_square_, destination_square_,
                                 &game_))) {
    if (p == game_.p1_) {
      turn_ = game_.p2_;
    } else {
      turn_ = game_.p1_;
    }
  } else {
    std::cout << "illegal";
  }
  ResetMoves();
}

void MyApp::draw() {
  DrawBoard();
}

void MyApp::mouseDown(MouseEvent event) {
  const board::Square* at = game_.board_->At(floor(event.getX() / kSquareSize),
                                      floor(event.getY() / kSquareSize));

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

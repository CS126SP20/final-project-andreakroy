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

void MyApp::setup() { origin_square_ = nullptr;
}

void MyApp::update() {}

void MyApp::draw() {
  DrawBoard();
/*  if (origin_square_) {
    cinder::gl::color(board::yellow);
    cinder::gl::drawSolidRect(origin_square_->loc_);
  }*/
}

void MyApp::mouseDown(MouseEvent event) {
  origin_square_ = game_.board_->At(floor(event.getX() / kSquareSize),
                                      floor(event.getY() / kSquareSize));
}

void MyApp::DrawBoard() {
  cinder::gl::clear();
  for (size_t j = 0; j < board::kSize; j++) {
    for (size_t i = 0; i < board::kSize; i++) {
      const board::Square* s = game_.board_->At(i, j);
      // When the square is not selected, set it to the appropriate color.
      cinder::gl::color(s->sq_color_);
      if (origin_square_ == s) {
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
}  // namespace myapp

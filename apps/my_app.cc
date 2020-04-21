// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

#include <cinder/app/App.h>
#include <cinder/gl/draw.h>
#include "cinder/gl/gl.h"
#include <chess/board.h>
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"

namespace myapp {

using cinder::app::MouseEvent;
using cinder::Rectf;
using board::kSquareSize;

MyApp::MyApp() { }

void MyApp::setup() {

}

void MyApp::update() { }

void MyApp::draw() {
  DrawBoard();
}

void MyApp::mouseDrag(MouseEvent event) {
  std::cout << event.getX();
}

void MyApp::DrawBoard() {
  for (size_t j = 0; j < board::kSize; j++) {
    for (size_t i = 0; i < board::kSize; i++) {
      cinder::gl::color(game_.board_->At(i, j)->sq_color_);
      cinder::gl::drawSolidRect(game_.board_->At(i, j)->loc_);
    }
  }
}
}  // namespace myapp

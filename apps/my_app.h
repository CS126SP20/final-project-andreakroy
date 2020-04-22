// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_MYAPP_H_
#define FINALPROJECT_APPS_MYAPP_H_

#include <cinder/app/App.h>
#include <chess/game.h>
#include <vector>
namespace myapp {

class MyApp : public cinder::app::App {
 public:
  MyApp();
  void setup() override;
  void update() override;
  void draw() override;
  void mouseDown(cinder::app::MouseEvent event) override;
  void DrawBoard();
 private:
  game::Game game_;
  const board::Square* origin_square_;
};

}  // namespace myapp

#endif  // FINALPROJECT_APPS_MYAPP_H_

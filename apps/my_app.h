// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_MYAPP_H_
#define FINALPROJECT_APPS_MYAPP_H_

#include <cinder/app/App.h>
#include <vector>
#include "chess/game.h"

namespace myapp {


class MyApp : public cinder::app::App {
  const long request_ok = 200;
 public:
  MyApp();
  void setup() override;
  void update() override;
  void draw() override;
  void mouseDown(cinder::app::MouseEvent event) override;
  void DrawBoard();
  void DrawGameOver();
 private:
  game::Game game_;
  game::Player* player_;
  const board::Square* origin_square_;
  const board::Square* destination_square_;
  game::Player* turn_;
  game::Move last_move_;
  std::string url_;
  game::GameState state_;
  void ResetMoves();
  void GetUpdate();
  void PostUpdate(const game::Move move);
};

}  // namespace myapp

#endif  // FINALPROJECT_APPS_MYAPP_H_

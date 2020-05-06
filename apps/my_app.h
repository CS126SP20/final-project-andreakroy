// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_MYAPP_H_
#define FINALPROJECT_APPS_MYAPP_H_

#include <cinder/app/App.h>
#include <vector>
#include <chess/game.h>

namespace myapp {

class MyApp : public cinder::app::App {
  // HTTP OK request code
  const long kRequestOK = 200;
 public:
  // Constructor for the Cinder app
  MyApp();
  void setup() override;
  void update() override;
  void draw() override;
  // Activated when the player clicks a square
  // The start and end moves of the square are set.
  void mouseDown(cinder::app::MouseEvent event) override;
  // Draws the board and pieces on the screen by rendering the piece images
  // and drawing a square chess grid. Called in draw().
  void DrawBoard();
  // Draws the ending screen of the game. Tells the player who won.
  void DrawGameOver();
 private:
  // The game object for this instance of the app.
  game::Game game_;
  // If in multiplayer mode, stores a pointer to the player who is playing
  // from this instance of the app.
  // If in single player mode, nullptr.
  game::Player* player_;
  // The color from whose point of view the board is in this instance of the
  // app. The board is oriented so that the player starts with his pieces at
  // the the bottom of the board and moves up the board.
  piece::Color pov_;
  // The origin square of a move. Turns yellow when a square is selected
  // given that it is a valid square for the player. nullptr when no square
  // has been selected.
  const board::Square* origin_square_;
  // The destination square of the move. If the destination square is valid,
  // a move is made to that square. nullptr if no destination square has been
  // selected.
  const board::Square* destination_square_;
  // The player whose turn it is in the game.
  game::Player* turn_;
  // The last move in the game stored as a move object.
  game::Move last_move_;
  // The game url. Empty string if the game is in single player mode/
  std::string url_;
  // The state of the game. Choices are in progress, drawn, white wins and
  // black wins.
  game::GameState state_;
  // Resets the origin and destination squares to their original states
  // (nullptr).
  void ResetMoves();
  // Receives an update from the server and updates the instance variables to
  // reflect the new game state.
  void GetUpdate();
  // Posts a legal move to the server.
  void PostUpdate(const game::Move move);
};
}  // namespace myapp

#endif  // FINALPROJECT_APPS_MYAPP_H_

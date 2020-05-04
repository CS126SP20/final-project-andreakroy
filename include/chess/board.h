//
// Created by andrea on 4/16/20.
//

#ifndef FINALPROJECT_BOARD_H
#define FINALPROJECT_BOARD_H

#include <cinder/app/App.h>
#include <cstddef>
#include "piece.h"

namespace board {

using piece::Piece;
using std::vector;
// The size of one dimension of a chess board.
const size_t kSize = 8;
const size_t kSquareSize = 100;
const cinder::Color green = {.4867f, .5843f, .17725f};
const cinder::Color white = cinder::Color::white();
const cinder::Color yellow {.859f, .850f, .100f};

class Board;

// Class representing one square on the board.
class Square {
 public:
  // Constructor creating the square object from a position (x, y) and a
  // pointer to a piece object.
  Square(const size_t x, const size_t y, Piece* p);
  // Square destructor.
  ~Square();
  // Square copy constructor.
  Square(const Square& other);
  // Square copy assignment operator.
  auto operator=(const Square& other) -> Square&;
  // Square constructor creating an empty square at position (x, y). An empty
  // square means that the piece field is set to a null ptr.
  Square(const size_t x, const size_t y);
  // Equality comparision operator checking if two squares point to the same
  // location on the chessboard.
  inline auto operator==(const Square& other) -> bool {return x_ == other.x_
                                                       && y_ == other.y_;}
  // Square inequality operator.
  inline auto operator!=(const Square& other) -> bool {return !(*this == other);}
  // Piece field holding a pointer to the piece on this square.
  Piece* piece_;
  // Integer from 0 through 7 representing the x coordinate on a chess board
  // if viewed as a set of Cartesian coordinates.
  size_t x_;
  // Integer from 0 through 7 representing the y coordinate on a chess board
  // if viewed as a set of Cartesian coordinates.
  size_t y_;
  // Color representing the color of the square (green or white)
  cinder::Color sq_color_;
  // Returns whether or not there is a piece at this square.
  auto IsEmpty() const -> bool;
};

class Board {
 private:
  // Grid storing all 64 squares on the board.
  Square* grid_[kSize * kSize];
 public:
  // Default board constructor. Returns a board with the default board setup.
  Board();
  // Board destructor.
  ~Board();
  // Board copy constructor.
  Board(const Board& other);
  // Board copy assignment operator.
  auto operator=(const Board& other) -> Board&;
  // Returns a const pointer to the square at position (x, y);
  auto At(const size_t x, const size_t y) const -> const Square*;
  // Sets the piece at the given square to the piece parameter.
  void Set(const Square* at, Piece* p);
  // Output stream operator to print a board to the console.
  friend auto operator << (std::ostream& out, const Board& b) -> std::ostream&;
};
}   // namespace board

#endif  // FINALPROJECT_BOARD_H

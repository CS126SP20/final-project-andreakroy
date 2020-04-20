//
// Created by andrea on 4/16/20.
//

#ifndef FINALPROJECT_BOARD_H
#define FINALPROJECT_BOARD_H

#include <chess/piece.h>
#include <cstddef>

namespace board {

using piece::Piece;
using std::vector;
// The size of one dimension of a chess board.
const size_t kSize = 8;

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
  Square& operator=(const Square& other);
  // Square constructor creating an empty square at position (x, y). An empty
  // square means that the piece field is set to a null ptr.
  Square(const size_t x, const size_t y);
  // Equality comparision operator checking if two squares point to the same
  // location on the chessboard.
  inline bool operator==(const Square& other) {return x_ == other.x_
                                                       && y_ == other.y_;}
  // Square inequality operator.
  inline bool operator!=(const Square& other) {return !(*this == other);}
  // Piece field holding a pointer to the piece on this square.
  Piece* piece_;
  // Integer from 0 through 7 representing the x coordinate on a chess board
  // if viewed as a set of Cartesian coordinates.
  size_t x_;
  // Integer from 0 through 7 representing the y coordinate on a chess board
  // if viewed as a set of Cartesian coordinates.
  size_t y_;
  // Returns whether or not there is a piece at this square.
  bool IsEmpty() const;
};
}   // namespace board


#endif  // FINALPROJECT_BOARD_H
// Copyright (c) 2020 Andrea Roy. All rights reserved.

#include <cassert>
#include <chess/piece.h>

using std::make_tuple;

namespace piece {
Piece::Piece(const PieceType t, const Color c) : type_(t), color_(c) {}

Pawn::Pawn(const Color c) : Piece(PieceType::kPawn, c) {}
auto Pawn::CanMove(const size_t x_old, const size_t y_old, const size_t x_new,
                   const size_t y_new) const -> bool {
  int x_diff = x_new - x_old;
  int y_diff = y_new - y_old;
  if (y_diff >= 0 && color_ == Color::kBlack) {
    // Checks that a black pawn is not moving backwards.
    return false;
  } else if (y_diff <= 0 && color_ == Color::kWhite) {
    // Checks that a white pawn is not moving backwards.
    return false;
  }
  // Gets the max size of a pawn move (either one or two squares) depending on
  // the pawn's original position.
  size_t max_y = 1;
  if ((y_old == 1 && color_ == Color::kWhite ||
       y_old == 6 && color_ == Color::kBlack) &&
      x_diff == 0) {
    max_y = 2;
  }
  if (abs(y_diff) > max_y) {
    // Checks that the pawn move is not exceeding the maximum allowable
    // distance.
    return false;
  } else if (abs(x_diff) > 1) {
    // Checks that the pawn move is not moving more than one square
    // horizontally.
    return false;
  }
  return true;
}
vector<tuple<size_t, size_t>> Pawn::Path(const size_t x_old, const size_t y_old,
                                         const size_t x_new,
                                         const size_t y_new) const {
  assert(CanMove(x_old, y_old, x_new, y_new));
  vector<tuple<size_t, size_t>> path;
  // Gets the maximum size of the move.
  size_t max_y = 1;
  if ((y_old == 1 && color_ == Color::kWhite ||
       y_old == 6 && color_ == Color::kBlack) &&
      x_old - x_new == 0) {
    max_y = 2;
  }
  // Stores the direction of movement.
  int factor = 1;
  if (color_ == Color::kBlack) {
    factor = -1;
  }
  for (int i = y_old + factor; i <= y_new; i += factor) {
    path.emplace_back(make_tuple(x_new, i));
  }
  return path;
}

}  // namespace piece

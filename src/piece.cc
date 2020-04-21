// Copyright (c) 2020 Andrea Roy. All rights reserved.

#include <chess/piece.h>
#include <cinder/app/App.h>
#include <cassert>

namespace piece {

using std::make_tuple;
using std::max;
using std::min;

Piece::Piece(const PieceType t, const Color c) : type_(t), color_(c), img_
                                                 (nullptr) {};
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

Knight::Knight(const Color c) : Piece(PieceType::kKnight, c) {}
bool Knight::CanMove(const size_t x_old, const size_t y_old, const size_t x_new,
                     const size_t y_new) const {
  // A knight geometrically moves exactly two squares in one direction and
  // one square in the other.
  size_t x_diff = abs(x_new - x_old);
  size_t y_diff = abs(y_new - y_old);
  return (max(x_diff, y_diff) == 2 && min(y_diff, x_diff) == 1);
}
vector<tuple<size_t, size_t>> Knight::Path(const size_t x_old,
                                           const size_t y_old,
                                           const size_t x_new,
                                           const size_t y_new) const {
  assert(CanMove(x_old, y_old, x_new, y_new));
  // A knight has a path consisting of its destination b/c it can jump over
  // anything.
  vector<tuple<size_t, size_t>> path;
  path.emplace_back(x_new, y_new);
  return path;
}

Bishop::Bishop(const Color c) : Piece(PieceType::kBishop, c) {}
bool Bishop::CanMove(const size_t x_old, const size_t y_old, const size_t x_new,
                     const size_t y_new) const {
  // A bishop must move exactly the same number of squares horizontally as
  // vertically.
  size_t x_diff = abs(x_new - x_old);
  size_t y_diff = abs(y_new - y_old);
  return x_diff == y_diff;
}
vector<tuple<size_t, size_t>> Bishop::Path(const size_t x_old,
                                           const size_t y_old,
                                           const size_t x_new,
                                           const size_t y_new) const {
  assert(CanMove(x_old, y_old, x_new, y_new));
  vector<tuple<size_t, size_t>> path;
  // A bishop's path consists of all squares along the diagonal.
  int xf = 1;
  int yf = 1;
  if (x_new - x_old < 0) {
    xf = -1;
  }
  if (y_new - y_old < 0) {
    yf = -1;
  }
  int y = y_old;
  for (int i = x_old + 1; i != x_new; i += xf) {
    path.emplace_back(make_tuple(i, y));
    y += yf;
  }
  return path;
}

Rook::Rook(const Color c) : Piece(PieceType::kRook, c) {}
bool Rook::CanMove(const size_t x_old, const size_t y_old, const size_t x_new,
                   const size_t y_new) const {
  int x_diff = x_new - x_old;
  int y_diff = y_new - y_old;
  // A valid rook move must be along a rank or file, so x_old == x_new or
  // y_new == y_old (but not both).
  return (x_diff == 0 != y_diff == 0);
}
vector<tuple<size_t, size_t>> Rook::Path(const size_t x_old, const size_t y_old,
                                         const size_t x_new,
                                         const size_t y_new) const {
  assert(CanMove(x_old, y_old, x_new, y_new));
  vector<tuple<size_t, size_t>> path;
  // The rook's path consists of every position along the rank or file.
  if (x_old == x_new) {
    for (int i = min(y_new, y_old) + 1; i <= max(y_new, y_old); i++) {
      path.emplace_back(make_tuple(x_new, i));
    }
  } else {
    for (int i = min(x_new, x_old) + 1; i <= max(x_new, x_old); i++) {
      path.emplace_back(make_tuple(i, y_new));
    }
  }
  return path;
}
Queen::Queen(const Color c) : Piece(piece::PieceType::kQueen, c){}
bool Queen::CanMove(const size_t x_old, const size_t y_old, const size_t x_new,
                    const size_t y_new) const {
  size_t x_diff = abs(x_new - x_old);
  size_t y_diff = abs(y_new - y_old);
  // A queen move is legal if the move is either a legal bishop move or a
  // valid rook move.
  return (x_diff == 0 != y_diff == 0) || x_diff == y_diff;
}

auto Queen::Path(const size_t x_old, const size_t y_old, const size_t x_new,
                 const size_t y_new) const -> vector<tuple<size_t, size_t>> {
  assert(CanMove(x_old, y_old, x_new, y_new));
  vector<tuple<size_t, size_t>> path;
  int xf = 1;
  int yf = 1;
  if (x_old == x_new) {
    for (int i = min(y_new, y_old) + 1; i <= max(y_new, y_old); i++) {
      path.emplace_back(make_tuple(x_new, i));
    }
    return path;
  } else if (y_old == y_new) {
    for (int i = min(x_new, x_old) + 1; i <= max(x_new, x_old); i++) {
      path.emplace_back(make_tuple(i, y_new));
    }
    return path;
  } else if (x_new - x_old < 0) {
    xf = -1;
  } else if (y_new - y_old < 0) {
    yf = -1;
  }
}

King::King(const Color c) : Piece(PieceType::kKing, c) {}
bool King::CanMove(size_t x_old, size_t y_old, size_t x_new,
                   size_t y_new) const {
  // Returns true if the king makes a legal move that is not castling.
  return (abs(x_new - x_old) <= 1 && abs(y_old - y_new) <= 1);
}

vector<tuple<size_t, size_t>> King::Path(size_t x_old, size_t y_old,
                                         size_t x_new, size_t y_new) const {
  assert(CanMove(x_old, y_old, x_new, y_new));
  // This method is useless, because castling needs to be checked against a
  // board anyway.
  vector<tuple<size_t, size_t>> path;
  path.emplace_back(make_tuple(x_new, y_new));
  return path;
}
}  // namespace piece

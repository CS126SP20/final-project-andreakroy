//
// Created by andrea on 4/16/20.
//

#ifndef FINALPROJECT_PIECE_H
#define FINALPROJECT_PIECE_H

#include <cstddef>
#include <tuple>
#include <vector>

using std::tuple;
using std::vector;

namespace piece {
enum class PieceType { kKing, kQueen, kRook, kPawn, kKnight, kBishop };

enum class Color { kBlack, kWhite };

class Piece {
 public:
  Piece(const PieceType t, const Color c);
  const PieceType type_;
  virtual bool CanMove(const size_t x_old, const size_t y_old,
                       const size_t x_new, const size_t y_new) const = 0;
  virtual vector<tuple<size_t, size_t>> Path(const size_t x_old,
                                             const size_t y_old,
                                             const size_t x_new,
                                             const size_t y_new) const = 0;
  const Color color_;
};

}  // namespace piece

#endif  // FINALPROJECT_PIECE_H

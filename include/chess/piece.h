//
// Created by andrea on 4/16/20.
//

#ifndef FINALPROJECT_PIECE_H
#define FINALPROJECT_PIECE_H

#include <cstddef>
#include <tuple>
#include <vector>

namespace piece {

using std::tuple;
using std::vector;

enum class PieceType { kKing, kQueen, kRook, kPawn, kKnight, kBishop };

enum class Color { kBlack, kWhite };

// Abstract Class representing a piece. Each peace has a type and color and
// implements CanMove and Path.
class Piece {
 public:
  // Piece constructor taking in a PieceType and Color enum.
  Piece(const PieceType t, const Color c);
  // The piece type.
  const PieceType type_;
  // Returns whether or not a piece could make a move from (x_old, y_old) to
  // (x_new, y_new). This method only checks the geometric  plausibility of
  // such a move and does not account for factors such as pieces being in the
  // way or the king being in check.
  virtual auto CanMove(const size_t x_old, const size_t y_old,
                       const size_t x_new, const size_t y_new) const
      -> bool = 0;
  // Returns a vector of (x,y) tuples representing positions on a chessboard.
  // Returns the hypothetical path a piece would take in travelling from
  // (x_old, y_old) to (x_new, y_new). Precondition: the move must be a valid
  // move.
  virtual auto Path(const size_t x_old, const size_t y_old, const size_t x_new,
                    const size_t y_new) const
      -> vector<tuple<size_t, size_t>> = 0;
  // The piece color (white or black).
  const Color color_;
};

// Class representing a pawn object.
class Pawn : public Piece {
 public:
  // Pawn override of the CanMove piece method.
  auto CanMove(const size_t x_old, const size_t y_old, const size_t x_new,
               const size_t y_new) const -> bool override;
  // Pawn override of the Path piece method.
  auto Path(const size_t x_old, const size_t y_old, const size_t x_new,
            const size_t y_new) const -> vector<tuple<size_t, size_t>> override;
  // Pawn constructor, creating a Pawn Piece with the given color.
  explicit Pawn(const Color c);
};

}  // namespace piece

#endif  // FINALPROJECT_PIECE_H

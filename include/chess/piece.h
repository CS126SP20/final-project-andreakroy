//
// Created by andrea on 4/16/20.
//

#ifndef FINALPROJECT_PIECE_H
#define FINALPROJECT_PIECE_H

#include <cstddef>
#include <tuple>
#include <vector>
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"

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
  // Cinder picture image object;
  const cinder::gl::Texture* img_;
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

// Class representing a knight object.
class Knight : public Piece {
 public:
  // Knight constructor.
  explicit Knight(Color c);
  // Knight override of the CanMove piece method.
  auto CanMove(const size_t x_old, const size_t y_old, const size_t x_new,
               const size_t y_new) const -> bool override;
  // Knight override of the Path piece method.
  auto Path(const size_t x_old, const size_t y_old, const size_t x_new,
            const size_t y_new) const -> vector<tuple<size_t, size_t>> override;
};

// Class representing a bishop object.
class Bishop : public Piece {
 public:
  // Bishop constructor.
  explicit Bishop(const Color c);
  // Bishop override of the CanMove piece method.
  auto CanMove(const size_t x_old, const size_t y_old, const size_t x_new,
               const size_t y_new) const -> bool override;
  // Bishop override of the Path piece method.
  auto Path(const size_t x_old, const size_t y_old, const size_t x_new,
            const size_t y_new) const -> vector<tuple<size_t, size_t>> override;
};

class Rook : public Piece {
 public:
  // Rook override of the CanMove piece method.
  auto CanMove(const size_t x_old, const size_t y_old, const size_t x_new,
               const size_t y_new) const -> bool override;
  // Rook override of the piece Path method.
  auto Path(const size_t x_old, const size_t y_old, const size_t x_new,
            const size_t y_new) const -> vector<tuple<size_t, size_t>> override;
  // Rook constructor.
  Rook(const Color c);
};

// Class representing a queen object.
class Queen : public Piece {
 public:
  // Queen override of the CanMove Piece method.
  auto CanMove(const size_t x_old, const size_t y_old, const size_t x_new,
               const size_t y_new) const -> bool override;
  // Queen override of the Path Piece method.
  auto Path(const size_t x_old, const size_t y_old, const size_t x_new,
            const size_t y_new) const -> vector<tuple<size_t, size_t>> override;
  // Queen constructor.
  explicit Queen(const Color c);
};

class King : public Piece {
 public:
  // King override of the Piece CanMove method.
  auto CanMove(const size_t x_old, const size_t y_old, const size_t x_new,
               const size_t y_new) const -> bool override;
  // King override of the Piece Path method.
  auto Path(const size_t x_old, const size_t y_old, const size_t x_new,
            const size_t y_new) const -> vector<tuple<size_t, size_t>> override;
  // King Constructor.
  explicit King(const Color c);
};
}  // namespace piece

#endif  // FINALPROJECT_PIECE_H

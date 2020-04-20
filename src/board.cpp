// Copyright (c) 2020 [Your Name]. All rights reserved.

#include <cassert>
#include <chess/board.h>
#include <chess/piece.h>
#include <ostream>

namespace board {

using piece::Piece;
using piece::Pawn;
using piece::Knight;
using piece::Rook;
using piece::Bishop;
using piece::King;
using piece::Queen;
using std::get;

Square::Square(const size_t x, const size_t y) {
  assert(x < kSize && y < kSize);
  x_ = x;
  y_ = y;
  piece_ = nullptr;
}
Square::~Square() { delete piece_; }

Square::Square(const Square& other) {
  x_ = other.x_;
  y_ = other.y_;
  if (other.piece_ == nullptr) {
    piece_ = nullptr;
  } else {
    switch (other.piece_->type_) {
      case piece::PieceType::kPawn:
        piece_ = new Pawn(other.piece_->color_);
        return;
      case piece::PieceType::kKnight:
        piece_ = new Knight(other.piece_->color_);
        return;
      case piece::PieceType::kBishop:
        piece_ = new Bishop(other.piece_->color_);
        return;
      case piece::PieceType::kRook:
        piece_ = new Rook(other.piece_->color_);
        return;
      case piece::PieceType::kKing:
        piece_ = new King(other.piece_->color_);
        return;
      case piece::PieceType::kQueen:
        piece_ = new King(other.piece_->color_);
        return;
    };
  }
}

Square& Square::operator=(const Square& other) {
  if (&other == this) {
    return *this;
  }
  x_ = other.x_;
  y_ = other.y_;
  delete piece_;
  if (other.piece_ == nullptr) {
    piece_ = nullptr;
  }
  switch (other.piece_->type_) {
    case piece::PieceType::kPawn:
      piece_ = new Pawn(other.piece_->color_);
      return *this;
    case piece::PieceType::kKnight:
      piece_ = new Knight(other.piece_->color_);
      return *this;
    case piece::PieceType::kBishop:
      piece_ = new Bishop(other.piece_->color_);
      return *this;
    case piece::PieceType::kRook:
      piece_ = new Rook(other.piece_->color_);
      return *this;
    case piece::PieceType::kKing:
      piece_ = new King(other.piece_->color_);
      return *this;
    case piece::PieceType::kQueen:
      piece_ = new King(other.piece_->color_);
      return *this;
  };
}
bool Square::IsEmpty() const { return piece_ == nullptr; }

Square::Square(size_t x, size_t y, Piece* p) {
  assert(x < kSize && y < kSize);
  x_ = x;
  y_ = y;
  piece_ = p;
}
}
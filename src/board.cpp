// Copyright (c) 2020 [Your Name]. All rights reserved.

#include <assert.h>
#include <chess/board.h>
#include <chess/piece.h>
#include <ostream>

using piece::Piece;
using piece::Pawn;
using piece::Knight;
using piece::Rook;
using piece::Bishop;
using piece::King;
using piece::Queen;
using std::get;

namespace board {

Square::Square(const size_t x, const size_t y) {
  assert(x < kSize && y < kSize);
  x_ = x;
  y_ = y;
  piece_ = nullptr;
}
Square::~Square() {
  delete piece_;
}

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

Board::Board() {
  piece::Color c = piece::Color::kWhite;
  //Initialize white back row (0)
  size_t row = 0;
  grid_[0] = new Square(0, row, new piece::Rook(c));
  grid_[1] = new Square(1, row, new piece::Knight(c));
  grid_[2] = new Square(2, row, new piece::Bishop(c));
  grid_[3] = new Square(3, row, new piece::Queen(c));
  grid_[4] = new Square(4, row, new piece::King(c));
  grid_[5] = new Square(5, row, new piece::Bishop(c));
  grid_[6] = new Square(6, row, new piece::Knight(c));
  grid_[7] = new Square(7, row, new piece::Rook(c));
  //Initialize white pawn row (1)
  row = 1;
  for (int i = 0; i < kSize; i++) {
    grid_[row * kSize + i] = new Square(i, row, new piece::Pawn(c));
  }
  //Initialize all middle empty rows (2 - 5 inclusive)
  for (int i = 2; i <= 5; i++) {
    for (int j = 0; j < kSize; j++) {
      grid_[i * kSize + j] = new Square(j, i);
    }
  }
  //Initialize black back row (7)
  c = piece::Color::kBlack;
  row = 7;
  grid_[0 + kSize * row] = new Square(0, row, new piece::Rook(c));
  grid_[1 + kSize * row] = new Square(1, row, new piece::Knight(c));
  grid_[2 + kSize * row] = new Square(2, row, new piece::Bishop(c));
  grid_[3 + kSize * row] = new Square(3, row, new piece::Queen(c));
  grid_[4 + kSize * row] = new Square(4, row, new piece::King(c));
  grid_[5 + kSize * row] = new Square(5, row, new piece::Bishop(c));
  grid_[6 + kSize * row] = new Square(6, row, new piece::Knight(c));
  grid_[7 + kSize * row] = new Square(7, row, new piece::Rook(c));
  row = 6;
  for (int i = 0; i < kSize; i++) {
    grid_[row * kSize + i] = new Square(i, row, new piece::Pawn(c));
  }
}
Board::~Board() {
  for (int i = 0; i < kSize * kSize; i++) {
    delete grid_[i];
  }
}
Board::Board(const Board& other) {
  for (int i = 0; i < kSize * kSize; i++) {
    grid_[i] = new Square(*other.grid_[i]);
  }
}
Board& Board::operator=(const Board& other) {
  if (&other == this) {
    return *this;
  }
  for (int i = 0; i < kSize * kSize; i++) {
    delete grid_[i];
    grid_[i] = new Square(*other.grid_[i]);
  }
  return *this;
}

const Square* Board::At(size_t x, size_t y) const {
  return grid_[kSize * y + x];
}

void Board::Set(const Square* at, Piece* pt) {
  assert(at != nullptr);
  Square* s = grid_[at->x_ + board::kSize * at->y_];
  if (pt == nullptr) {
    s->piece_ = nullptr;
    return;
  }
  s->piece_ = pt;
}

std::ostream& operator<<(std::ostream& os, const Board& b) {
  for (int j = 0; j < kSize; j++) {
    for (int i = 0; i < kSize; i++) {
      if (b.At(i, j)->piece_ == nullptr) {
        os << "    ";
        continue;
      }
      piece::PieceType p = b.At(i, j)->piece_->type_;
      piece::Color c = b.At(i, j)->piece_->color_;
      switch (c) {
        case piece::Color::kBlack:
          os << "B";
          break;
        case piece::Color::kWhite:
          os << "W";
          break;
      };
      switch (p) {
        case piece::PieceType::kPawn:
          os << "P  ";
          continue;
        case piece::PieceType::kKing:
          os << "K  ";
          continue;
        case piece::PieceType::kQueen:
          os << "Q  ";
          continue;
        case piece::PieceType::kRook:
          os << "R  ";
          continue;
        case piece::PieceType::kBishop:
          os << "B  ";
          continue;
        case piece::PieceType::kKnight:
          os << "N  ";
          continue;
      }
    }
    os << "\n";
  }
  return os;
}
}  // namespace board
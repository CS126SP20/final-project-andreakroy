//
// Created by andrea on 4/18/20.
//

#include "chess/game.h"

#include <assert.h>

#include <iostream>

#include "chess/piece.h"

namespace game {

using std::get;
using piece::Knight;
using piece::Pawn;
using piece::King;
using piece::Rook;
using piece::Queen;
using piece::Bishop;
using std::tuple;

Player::Player(const piece::Color c, const Square* king) {
  color_ = c;
  numPieces_ = 16;
  HasKingMoved_ = false;
  IsKingInCheck_ = false;
  kingSquare_ = king;
}

Move Player::PlayMove(const Square *from, const Square *to, Game* game) {
  if (!from || !to || !game) {
    return {this, nullptr, nullptr, 0, false};
  }
  size_t move_number = game->move_number_;
  if (from->piece_->color_ == piece::Color::kWhite) {
    ++move_number;
  }
  if (from->piece_->type_ == piece::PieceType::kKing) {
    if (abs(static_cast<int>(from->x_) - (static_cast<int>(to->x_))) > 1) {
      return {this, from, to, move_number, true};
    }
  }
  return {this, from, to, move_number, false};
}

Game::Game(const int id) {
  white_ = new Player(piece::Color::kWhite, nullptr);
  black_ = new Player(piece::Color::kBlack, nullptr);
  board_ = new Board();
  white_->kingSquare_ = board_->At(4, 0);
  black_->kingSquare_ = board_->At(4, 7);
  moves_ = vector<Move>();
  id_ = id;
  move_number_ = 0;
}

Game::~Game() {
  delete white_;
  delete black_;
  delete board_;
}

Game::Game(const Game &other) {
  white_ = new Player(*other.white_);
  black_ = new Player(*other.black_);
  board_ = new Board(*other.board_);
  id_ = other.id_;
  move_number_ = other.move_number_;
  moves_ = other.moves_;
}

auto Game::operator=(const Game &other) -> Game & {
  if (&other == this) {
    return *this;
  }
  delete white_;
  delete black_;
  delete board_;
  white_ = new Player(*other.white_);
  black_ = new Player(*other.black_);
  board_ = new Board(*other.board_);
  id_ = other.id_;
  move_number_ = other.move_number_;
  moves_ = other.moves_;
  return *this;
}

auto Game::CanMove(const Square* from, const Square* to, Player* p) const -> bool {
  if (!from || !to || !p) {
    return false;
  }
  if (from == to) {
    return false;
  }
  //Can't capture a piece of the same color
  if (to->piece_ != nullptr && to->piece_->color_ == from->piece_->color_) {
    return false;
  }

  switch(from->piece_->type_) {
    case piece::PieceType::kBishop:
      return from->piece_->CanMove(from->x_, from->y_, to->x_, to->y_) &&
             CheckPath(from, to);
    case piece::PieceType::kKnight:
      return from->piece_->CanMove(from->x_, from->y_, to->x_, to->y_);
    case piece::PieceType::kRook:
      return from->piece_->CanMove(from->x_, from->y_, to->x_, to->y_) &&
             CheckPath(from, to);
    case piece::PieceType::kQueen:
      return from->piece_->CanMove(from->x_, from->y_, to->x_, to->y_) &&
             CheckPath(from, to);
    case piece::PieceType::kKing:
      return from->piece_->CanMove(from->x_, from->y_, to->x_, to->y_) &&
             CheckPath(from, to);
    case piece::PieceType::kPawn:
      if (!from->piece_->CanMove(from->x_, from->y_, to->x_, to->y_)) {
        return false;
      }
      // Can't move straight ahead if there is a piece there
      if (from->x_ - to->x_ == 0 && !to->IsEmpty()) {
        return false;
      }
      return true;
    default:
      return false;
  }
}

auto Game::PlayTurn(const Move m) -> bool {
  //If the king is in check and the piece to move is not the king, return false
  if (m.player_->IsKingInCheck_) {
    if (m.from_->piece_->type_ != piece::PieceType::kKing) {
      // If after the proposed move the king would still be in check return false.
      if (m.to_->IsEmpty() || m.to_->piece_->color_ != m.player_->color_) {
        board_->Set(m.from_, nullptr);
        board_->Set(m.to_, m.from_->piece_);
      }
      if (WouldKingBeInCheck(m.player_->kingSquare_, m.player_->color_)) {
        board_->Set(m.from_, m.from_->piece_);
        board_->Set(m.to_, m.to_->piece_);
        return false;
      }
      board_->Set(m.from_, m.from_->piece_);
      board_->Set(m.to_, m.to_->piece_);
    } else {
      if (WouldKingBeInCheck(m.to_, m.player_->color_)) {
        return false;
    }
  }
}

if (!CanMove(m.from_, m.to_, m.player_)) {
    return false;
  }
  if (m.from_->piece_->type_ == piece::PieceType::kPawn) {
    if (m.from_->x_ != m.to_->x_ && !m.to_->piece_) {
      return false;
    }
  }
  if (m.from_->piece_->type_ == piece::PieceType::kKing) {
    if (m.IsCastling_ && CanCastle(m.player_, m.to_)) {
      if (m.from_->x_ > m.to_->x_) {
        board_->Set(m.to_, m.from_->piece_);
        board_->Set(m.from_, nullptr);
        board_->Set(board_->At(m.to_->x_ + 1, m.from_->y_),
                    board_->At(0, m.from_->y_)->piece_);
        board_->Set(board_->At(0, m.from_->y_), nullptr);
      } else {
        board_->Set(m.to_, m.from_->piece_);
        board_->Set(m.from_, nullptr);
        board_->Set(board_->At(m.to_->x_ - 1, m.from_->y_),
                    board_->At(board::kSize - 1, m.from_->y_)->piece_);
        board_->Set(board_->At(board::kSize - 1, m.from_->y_), nullptr);
      }
      m.player_->kingSquare_ = m.to_;
      moves_.emplace_back(m);
      return true;
    }
    if (WouldKingBeInCheck(m.to_, m.from_->piece_->color_)) {
      return false;
    }

    m.player_->kingSquare_ = m.to_;
  }

  if (m.from_->piece_->type_ == piece::PieceType::kRook) {
    if (m.from_->x_ == 0) {
      m.player_->HasQueenRookMoved_ = true;
    } else if (m.from_->y_ == 7) {
      m.player_->HasKingRookMoved_ = true;
    }
  }
  board_->Set(m.to_, m.from_->piece_);
  board_->Set(m.from_, nullptr);
  if (WouldKingBeInCheck(white_->kingSquare_, piece::Color::kWhite)) {
    white_->IsKingInCheck_ = true;
  } else if (WouldKingBeInCheck(black_->kingSquare_, piece::Color::kBlack)) {
    black_->IsKingInCheck_ = true;
  } else {
    white_->IsKingInCheck_ = false;
    black_->IsKingInCheck_ = false;
  }
  moves_.emplace_back(m);
  if (m.to_->piece_ != nullptr) {
    if (m.player_->color_ == piece::Color::kWhite) {
      black_->numPieces_--;
    } else {
      white_->numPieces_--;
    }
  }

  // If the player is white, increment the move counter
  if (m.player_ == white_) {
    move_number_++;
  }

  return true;
}

auto Game::CheckPath(const Square* from, const Square* to) const -> bool {
  assert(from != to);
  assert(!from->IsEmpty());
  const Square* s;
  for (tuple<size_t, size_t> it : from->piece_->Path(from->x_, from->y_,
                                                    to->x_, to->y_)) {
    s = board_->At(get<0>(it), get<1>(it));
    if (!s->IsEmpty() && s != to) {
      return false;
    }
  }
  return true;
}
auto Game::WouldKingBeInCheck(const Square* at, piece::Color c) const -> bool {
  for (int j = board::kSize - 1; j >= 0; j--) {
    for (int i = 0; i < board::kSize; i++) {
      const Square* s = board_->At(i, j);
      if (s->IsEmpty() || s->piece_->color_ == c) {
        continue;
      }
      //If a piece can move to the king's square and is of the opposite
      // color, the king is in check.
      Player* p = white_;
      if (c == piece::Color::kWhite) {
        p = black_;
      }
      piece::Piece* temp = at->piece_;
      if (temp && temp->color_ != p->color_) {
        board_->Set(at, nullptr);
      }
      if (CanMove(s, at, p)) {
        board_->Set(at, temp);
        return true;
      }
      board_->Set(at, temp);
    }
  }
  return false;
}

auto Game::GetAllPossibleKingMoves(Player* p)
    const -> vector<const Square*> {
  vector<const Square*> moves;
  int maxKingMove = 1;
  int minKingMove = -1;
  int test_x;
  int test_y;
  for (int i = minKingMove; i <= maxKingMove; i++) {
    for (int j = minKingMove; j <= maxKingMove; j++) {
      test_x = p->kingSquare_->x_ + i;
      test_y = p->kingSquare_->y_ + j;
      if ((i == 0 && j == 0) || test_x < 0 || test_y < 0 || test_x >=
          board::kSize || test_y >= board::kSize) {
        continue;
      }
      const Square* test = board_->At(test_x,test_y);
      piece::Piece* temp = test->piece_;
      if (temp && temp->color_ != p->color_) {
        board_->Set(test, nullptr);
      }
      if (CanMove(p->kingSquare_, test, p) && !WouldKingBeInCheck(test, p->color_)) {
        board_->Set(test, temp);
        moves.emplace_back(test);
      }
      board_->Set(test, temp);
    }
  }
  return moves;
}

auto Game::EvaluateBoard() const -> GameState {
  if (GetAllPossibleKingMoves(white_).empty()) {
    if (white_->IsKingInCheck_) {
      return GameState::kBlackWin;
    } else if (white_->numPieces_ == 0) {
      return GameState::kDraw;
    }
  } else if (GetAllPossibleKingMoves(black_).empty()) {
    if (black_->IsKingInCheck_) {
      return GameState::kWhiteWin;
    } else if (black_-> numPieces_ == 0) {
      return GameState::kDraw;
    }
  }
  return GameState::kIP;
}

auto Game::CanCastle(const Player* p, const Square* s) const -> bool {
  //Check that the square to go to is the correct kingside square(6) or
  // queenside square(6).
  if (s->x_ != 2 && s->x_ != 6) {
    return false;
  }
  bool kingSide;
  if (s->x_ == 6) {
    kingSide = true;
  }
  size_t row;
  if (p == white_) {
    //If the player is white make sure castling on the back row.
    if (s->y_ != 0) {
      return false;
    }
    row = 0;
  } else {
    //If the player is black make sure castling on back row.
    if (s->y_ != 7) {
      return false;
    }
    row = 7;
  }
  if (p->HasKingMoved_) {
    //Can't castle if the king has moved.
    return false;
  } else if (p->IsKingInCheck_) {
    //Can't castle if the king is in check.
    return false;
  } else if (p->HasKingRookMoved_ && kingSide) {
    //Can't castle kingside if the king rook has moved.
    return false;
  } else if (p->HasQueenRookMoved_ && !kingSide) {
    //Can't castle queenside if the queen rook has moved.
    return false;
  }
  if (kingSide) {
    //Look at every square between the start and end square to see if the
    // king would be in check.
    for (int x_pos = p->kingSquare_->x_ + 1; x_pos <= s->x_; x_pos ++) {
      const Square* intermediate = board_->At(x_pos, s->y_);
      if (!intermediate->IsEmpty()) {
        //Can't castle if there are pieces at intermediate squares.
        return false;
      } else if (WouldKingBeInCheck(intermediate, p->color_)) {
        //Can't castle through check.
        return false;
      }
    }
  }
  return true;
}
auto Game::GetMoveFromStr(const std::string str, Player* p) -> Move {
  assert(str.size() == 4);
  int offset = '0';
  const Square* from = board_->At(str.at(0) - offset, str.at(1) - offset);
  const Square* to =  board_->At(str.at(2) - offset, str.at(3) - offset);
  return p->PlayMove(from, to, this);
}

std::ostream &operator << (std::ostream &os, const Move &move) {
  if (move.from_ == nullptr || move.to_ == nullptr) {
    return os;
  }
  os << move.from_->x_;
  os << move.from_->y_;
  os << move.to_->x_;
  os << move.to_->y_;
  return os;
}
}  // namespace game
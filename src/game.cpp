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
  assert(!from->IsEmpty());
  assert(from->piece_->color_ == color_);
  if (from ->piece_->type_ == piece::PieceType::kKing) {
    return {this, from, to, false, MoveState::kIllegal};
  } else {
    if (!game->CanMove(from, to, this)) {
      return {this, from, to, false, MoveState::kIllegal};
    } else {
      return {this, from, to, false, MoveState::kSuccess};
    }
  }
}

Game::Game() : p1_(Player(piece::Color::kWhite, nullptr)),
               p2_(Player(piece::Color::kBlack, nullptr)) {
  board_ = Board();
  p1_.kingSquare_ = board_.At(4, 0);
  p2_.kingSquare_ = board_.At(7, 4);
  moves_ = vector<Move>();
}


bool Game::CanMove(const Square* from, const Square* to, Player* p) const {
  assert(!from->IsEmpty());
  if (from == to) {
    return false;
  }
  //Can't capture a piece of the same color
  if (to->piece_ != nullptr && to->piece_->color_ == from->piece_->color_) {
    return false;
  }
  //If the king is in check and the piece to move is not the king, return false
/*  if (WouldKingBeInCheck(p->kingSquare_, p->color_) && from->piece_->type_ !=
   piece::PieceType::kKing) {
    return false;
  }*/
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
      if (WouldKingBeInCheck(to, p->color_)) {
        return false;
      } else if ((from->x_ - to->x_ == 3 || abs(from->x_ - to->x_) == 2) &&
                 !CanCastle(p, to)) {
        return false;
      } else if (!from->piece_->CanMove(from->x_, from->y_, to->x_, to->y_)) {
        return false;
      } else if (WouldKingBeInCheck(to, p->color_)) {
        return false;
      }
      return true;
    case piece::PieceType::kPawn:
      if (!from->piece_->CanMove(from->x_, from->y_, to->x_, to->y_)) {
        return false;
      }
      //Can't move horizontally if not capturing a piece
      if (from->x_ - to->x_ != 0 && to->IsEmpty()) {
        return false;
      }
      return true;
    default:
      return false;
  }
}

bool Game::PlayTurn(const Move m) {
  if (m.state_ == MoveState::kSuccess) {
    if (m.to_->piece_ != nullptr) {
      if (m.player_->color_ == piece::Color::kWhite) {
        p2_.numPieces_--;
      } else {
        p1_.numPieces_--;
      }
    }
    if (m.from_->piece_->type_ == piece::PieceType::kKing) {
      if (m.player_->color_ == piece::Color::kWhite) {
        p1_.kingSquare_ == m.to_;
        p1_.HasKingRookMoved_ = true;
      } else {
        p2_.kingSquare_ == m.to_;
        p2_.HasKingRookMoved_ = true;
      }
    }
    if (m.from_->piece_->type_ == piece::PieceType::kRook) {
      if (m.from_->x_ == 0) {
        m.player_->HasQueenRookMoved_ = true;
      } else if (m.from_->y_ == 7) {
        m.player_->HasKingRookMoved_ = true;
      }
    }
    board_.Set(m.to_, m.from_->piece_);
    board_.Set(m.from_, nullptr);
    moves_.push_back(m);
    return true;
  }
  return false;
}

bool Game::CheckPath(const Square* from, const Square* to) const {
  assert(&from != &to);
  assert(!from->IsEmpty());
  for (tuple<size_t, size_t> it : from->piece_->Path(from->x_, from->y_,
                                                    to->x_, to->y_)) {
    const Square* s = board_.At(get<0>(it), get<1>(it));
    if (s->IsEmpty()) {
      continue;
    }
    if (s->piece_ != nullptr && s != to) {
      return false;
    }
  }
  return true;
}
bool Game::WouldKingBeInCheck(const Square* at, piece::Color c) const {
  for (int j = board::kSize - 1; j >= 0; j--) {
    for (int i = 0; i < board::kSize; i++) {
      const Square* s = board_.At(i, j);
      if (s->IsEmpty() || s->piece_->color_ == c) {
        continue;
      }
      //If a piece can move to the king's square and is of the opposite
      // color, the king is in check.
      if (s->piece_->CanMove(s->x_, s->y_, at->x_,at->y_)) {
        return true;
      }
    }
  }
  return false;
}

vector<const Square*> Game::GetAllPossibleMoves(const Square* s, Player* p)
    const {
  vector<const Square*> moves;
  assert(s != nullptr);
  assert(s->piece_ != nullptr);
  assert(s->piece_->color_ == p->color_);
  for (size_t j = 0; j < board::kSize; j++) {
    for (size_t i = 0; i < board::kSize; i++) {
      if (CanMove(board_.At(s->x_, s->y_), board_.At(i, j), p)) {
        moves.push_back(board_.At(i, j));
      }
    }
  }
  return moves;
}

GameState Game::Run() {
  Player* p = &p1_;
  PlayTurn(p1_.PlayMove(board_.At(4, 1), board_.At(4, 3), this));
  PlayTurn(p2_.PlayMove(board_.At(4, 6), board_.At(4, 4), this));
  PlayTurn(p1_.PlayMove(board_.At(1, 0), board_.At(2, 2), this));

}
GameState Game::EvaluateBoard() const {
  return GameState::kIP;
}

bool Game::CanCastle(const Player* p, const Square* s) const {
  //Check that the square to go to is the correct kingside square(6) or
  // queenside square(6).
  assert(s->x_ == 2 || s->x_ == 6);
  bool kingSide;
  if (s->x_ == 6) {
    kingSide = true;
  }
  size_t row;
  if (p = &p1_) {
    //If the player is white make sure castling on the back row.
    assert(s->y_ == 0);
    row = 0;
  } else {
    //If the player is black make sure castling on back row.
    assert(s->y_ == 7);
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
      const Square* intermediate = board_.At(x_pos, s->y_);
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
}  // namespace game
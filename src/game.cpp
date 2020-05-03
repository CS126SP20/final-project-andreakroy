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
  kingSquare_ = king;
}

auto Player::PlayMove(const Square *from, const Square *to, Game* game) -> Move {
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

auto Player::IsKingInCheck() -> const bool {
  return !PiecesChecking_.empty();
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
  if (!from || !from->piece_ || !to || !p) {
    return false;
  }
  if (from == to) {
    return false;
  }
  //Can't capture a piece of the same color
  if (to->piece_ && to->piece_->color_ == from->piece_->color_) {
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
      if (from->x_ != to->x_ && !to->piece_) {
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
  piece::Piece* from_temp = m.from_->piece_;
  piece::Piece* to_temp = m.to_->piece_;
  bool isKingMove;
  const Square* last_king_square = m.player_->kingSquare_;
  if (from_temp->type_ == piece::PieceType::kKing) {
    isKingMove = true;
  }

  if (m.IsCastling_ && !CanCastle(m.player_, m.to_)) {
    return false;
  }

  // try to make the move.
  board_->Set(m.to_, from_temp);
  board_->Set(m.from_, nullptr);
  if (isKingMove) {
    m.player_->kingSquare_ = m.to_;
  }
  // if the king is in check
  if (!GetPiecesChecking(m.player_->kingSquare_, m.player_).empty()) {
    // clean up and return false
   board_->Set(m.to_, to_temp);
   board_->Set(m.from_, from_temp);
   if (isKingMove) {
     m.player_->kingSquare_ = last_king_square;
   }
   return false;
  }
  board_->Set(m.to_, to_temp);
  board_->Set(m.from_, from_temp);


  if (!CanMove(m.from_, m.to_, m.player_)) {
    return false;
  }

  // ************* By this point, the move is legal **************

  // If the move is by a rook, update the player rook movement vars
  if (from_temp->type_ == piece::PieceType::kRook) {
    if (m.from_->x_ == 0) {
      m.player_->HasQueenRookMoved_ = true;
    } else if (m.from_->y_ == 7) {
      m.player_->HasKingRookMoved_ = true;
    }
  }

  // If the move was a capture, update the number of pieces
  if (to_temp) {
    if (m.player_->color_ == piece::Color::kWhite) {
      black_->numPieces_--;
    } else {
      white_->numPieces_--;
    }
  }

  // If the player was white, increment the number of moves
  if (m.player_ == white_) {
    ++move_number_;
  }

  if (from_temp->type_ == piece::PieceType::kKing) {
    m.player_->HasKingRookMoved_ = true;
    m.player_->kingSquare_ = m.to_;
  }

  if (m.IsCastling_) {
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
  } else {
    board_->Set(m.to_, from_temp);
    board_->Set(m.from_, nullptr);
  }

  // update player check tracker
  white_->PiecesChecking_ = GetPiecesChecking(white_->kingSquare_, white_);
  black_->PiecesChecking_ = GetPiecesChecking(black_->kingSquare_, black_);
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

auto Game::GetPiecesChecking(const Square* at, Player* player) const ->
    vector<const Square*> {
  //If a piece can move to the king's square and is of the opposite
  // color, the king is in check.
  vector<const Square*> squares;
  piece::Piece* temp = at->piece_;
  board_->Set(at, nullptr);
  const Square* sq;
  Player* p = white_;
  if (p == player) {
    p = black_;
  }
  for (int j = 0; j < board::kSize; j++) {
    for (int i = 0; i < board::kSize; i++) {
      sq = board_->At(i, j);
      if (!sq->piece_ || sq->piece_->color_ == player->color_) {
        continue;
      }
      if (CanMove(sq, at, p)) {
        squares.emplace_back(sq);
      }
    }
  }
  board_->Set(at, temp);
  return squares;
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
      if (CanMove(p->kingSquare_, test, p) && !GetPiecesChecking(test, p).empty
                                               ()) {
        board_->Set(test, temp);
        moves.emplace_back(test);
      }
      board_->Set(test, temp);
    }
  }
  return moves;
}

auto Game::EvaluateBoard() const -> GameState {
  if (!GetAllPossibleKingMoves(white_).empty()) {
    if (white_->PiecesChecking_.empty()) {
      return GameState::kBlackWin;
    } else if (white_->numPieces_ == 0) {
      return GameState::kDraw;
    }
  } else if (GetAllPossibleKingMoves(black_).empty()) {
    if (!black_->PiecesChecking_.empty()) {
      return GameState::kWhiteWin;
    } else if (black_-> numPieces_ == 0) {
      return GameState::kDraw;
    }
  }
  return GameState::kIP;
}

auto Game::CanCastle(Player* p, const Square* s) const -> bool {
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
    if (s->y_ != board::kSize - 1) {
      return false;
    }
    row = 7;
  }
  if (p->HasKingMoved_) {
    //Can't castle if the king has moved.
    return false;
  } else if (!p->PiecesChecking_.empty()) {
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
        // Can't castle if there are pieces at intermediate squares.
        return false;
      } else if (!GetPiecesChecking(intermediate, p).empty()) {
        // Can't castle through check.
        return false;
      }
    }
  } else {
    //Look at every square between the start and end square to see if the
    // king would be in check.
    for (int x_pos = s->x_; x_pos < p->kingSquare_->x_; x_pos ++) {
      const Square* intermediate = board_->At(x_pos, s->y_);
      if (!intermediate->IsEmpty()) {
        //Can't castle if there are pieces at intermediate squares.
        return false;
      } else if (!GetPiecesChecking(intermediate, p).empty()) {
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
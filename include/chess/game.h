
//
// Created by andrea on 4/18/20.
//

#ifndef FINALPROJECT_GAME_H
#define FINALPROJECT_GAME_H
#include "board.h"
#include "piece.h"


namespace game {

using board::Board;
using board::Square;
using std::vector;

// Enum class representing all the possible game states.
enum class GameState {
  kDraw,
  kIP,
  kP1Win,
  kP2Win
};

// Enum class representing all possible move states.
enum class MoveState {
  kSuccess,
  kIllegal
};

// Player and Game class forward declarations.
class Player;
class Game;

// Move struct to compactly return a player's attempted move.
struct Move {
  // The player who made the move.
  Player* player_;
  // The square from which the piece moved.
  const Square* from_;
  // The square to which the piece moved.
  const Square* to_;
  // True if the move was a castling move, false otherwise.
  bool IsCastling_;
  // Whether the move was successful or illegal.
  MoveState state_;
};

// Class representing a player. Each game has two players.
class Player {
 public:
  // Player constructor.
  Player(const piece::Color c, const Square* king);
  // The color which the player is playing.
  piece::Color color_;
  // Function to allow the player to attempt to make a move. Returns a struct
  // packaging the move metadata.
  auto PlayMove(const Square* from, const Square* to, Game* game) -> Move;
  // true iff the player's king has moved during the course of the game.
  bool HasKingMoved_;
  // true iff the king is in check.
  bool IsKingInCheck_;
  // true iff the king rook has moved.
  bool HasKingRookMoved_;
  // true iff the queen rook has moved.
  bool HasQueenRookMoved_;
  // the number of pieces the player has on the board.
  size_t numPieces_;
  // a pointer to the current square which the king is on.
  const Square* kingSquare_;
};

class Game {
 public:
  // Default game constructor.
  Game();
  //Game Destructor.
  ~Game();
  // Game copy constructor.
  Game(const Game& other);
  // Game copy assignment operator.
  auto operator=(const Game& other) -> Game&;
  //Copy Assignment Operator.
  // A vector of all successful moves in the game thus far.
  vector<Move> moves_;
  // A board object representing the board of the game.
  Board* board_;
  // Player 1 i.e. the white player.
  Player* p1_;
  // Player 2 i.e. the black player.
  Player* p2_;
  // True when it is player 1's turn , false otherwise.
  bool IsP1Turn_;
  // Returns the current game state of the board.
  auto EvaluateBoard() const -> GameState;
  // Plays the game. Returns the end result as a GameState.
  auto Run() -> GameState;
  // Takes in a move object for a player Move and returns true and updates
  // the board if the move was successful.
  auto PlayTurn(const Move m) -> bool;
  // Returns true if the player can legally make a move from a square to
  // another square.
  auto CanMove(const Square* from, const Square* to, Player* p) const -> bool;
 private:
  // Checks whether the piece's path tries to run over an existing piece in a
  // move from a square to another.
  auto CheckPath(const Square* from, const Square *to) const -> bool;
  // Returns true if the king of the given color would be in check from the
  // other player's pieces at a given square.
  auto WouldKingBeInCheck(const Square* at, piece::Color c) const -> bool;
  // Gets a vector of pointers to squares of all possible moves by a player
  // from a square.
  auto GetAllPossibleMoves(const Square* from, Player* p)
      const -> vector<const Square*>;
  // Returns true if the player can make a castling move to the given square.
  auto CanCastle(const Player* p, const Square* s) const -> bool;
  // Gets a move from a string
  auto GetMoveFromStr(std::string str, Player* p) -> Move;
};
}  // namespace game


#endif  // FINALPROJECT_GAME_H


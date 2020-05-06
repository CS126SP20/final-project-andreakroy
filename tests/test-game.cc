// Copyright (c) 2020 Andrea Roy. All rights reserved.

#define CATCH_CONFIG_MAIN

#include <chess/game.h>
#include <catch2/catch.hpp>

TEST_CASE("New Game Setup", "[board][game][default]") {
  game::Game game(0);
  // check the kings are in the proper place
  REQUIRE(game.board_->At(4, 0)->piece_->type_ == piece::PieceType::kKing);
  REQUIRE(game.board_->At(4, 7)->piece_->type_ == piece::PieceType::kKing);
  // check all the middle squares are empty
  for (size_t i = 0; i < board::kSize; i++) {
    for (size_t j = 2; j < 6; j++) {
       REQUIRE(game.board_->At(i, j)->IsEmpty());
    }
  }
  // check the queens are in the proper places
  REQUIRE(game.board_->At(3, 0)->piece_->type_ == piece::PieceType::kQueen);
  REQUIRE(game.board_->At(3, 7)->piece_->type_ == piece::PieceType::kQueen);
  // check the bishops are in the proper places
  REQUIRE(game.board_->At(2, 0)->piece_->type_ == piece::PieceType::kBishop);
  REQUIRE(game.board_->At(5, 0)->piece_->type_ == piece::PieceType::kBishop);
  REQUIRE(game.board_->At(2, 7)->piece_->type_ == piece::PieceType::kBishop);
  REQUIRE(game.board_->At(5, 7)->piece_->type_ == piece::PieceType::kBishop);
  // check the rooks are in the proper places
  REQUIRE(game.board_->At(0, 0)->piece_->type_ == piece::PieceType::kRook);
  REQUIRE(game.board_->At(7, 0)->piece_->type_ == piece::PieceType::kRook);
  REQUIRE(game.board_->At(0, 7)->piece_->type_ == piece::PieceType::kRook);
  REQUIRE(game.board_->At(7, 7)->piece_->type_ == piece::PieceType::kRook);
  // check the knights are in the proper places
  REQUIRE(game.board_->At(1, 0)->piece_->type_ == piece::PieceType::kKnight);
  REQUIRE(game.board_->At(6, 0)->piece_->type_ == piece::PieceType::kKnight);
  REQUIRE(game.board_->At(1, 7)->piece_->type_ == piece::PieceType::kKnight);
  REQUIRE(game.board_->At(6, 7)->piece_->type_ == piece::PieceType::kKnight);
  // check that all the pawns are in the right places
  for (size_t i = 0; i < board::kSize; i++) {
    REQUIRE(game.board_->At(i, 1)->piece_->type_ == piece::PieceType::kPawn);
  }
  for (size_t i = 0; i < board::kSize; i++) {
    REQUIRE(game.board_->At(i, 6)->piece_->type_ == piece::PieceType::kPawn);
  }
}

TEST_CASE("Test En Passant", "[game][en-passant]") {
  game::Game game(0);
  game.PlayTurn(game.white_->PlayMove(game.board_->At(4, 1),
                                      game.board_->At(4, 3), &game));
  game.PlayTurn(game.black_->PlayMove(game.board_->At(6, 6),
                                      game.board_->At(6, 4), &game));
  game.PlayTurn(game.white_->PlayMove(game.board_->At(4, 3),
                                      game.board_->At(4, 4), &game));
  game.PlayTurn(game.black_->PlayMove(game.board_->At(5, 6),
                                      game.board_->At(5, 4), &game));
  REQUIRE(game.PlayTurn(game.white_->PlayMove(game.board_->At(4, 4),
                                              game.board_->At(5, 5), &game)));
  REQUIRE(game.board_->At(5, 4)->IsEmpty());
}

TEST_CASE("Test Checkmate", "[game][checkmate]") {
  SECTION("Test Fool's Mate") {
    game::Game game(0);
    game.PlayTurn(game.white_->PlayMove(game.board_->At(6, 1),
                                        game.board_->At(6, 3), &game));
    game.PlayTurn(game.black_->PlayMove(game.board_->At(4, 6),
                                        game.board_->At(4, 4), &game));
    game.PlayTurn(game.white_->PlayMove(game.board_->At(5, 1),
                                        game.board_->At(5, 2), &game));
    game.PlayTurn(game.black_->PlayMove(game.board_->At(3, 7),
                                        game.board_->At(7, 3), &game));
    REQUIRE(game.EvaluateBoard() == game::GameState::kBlackWin);
  }
  SECTION("Test 4 move Checkmate") {
    game::Game game(0);
    game.PlayTurn(game.white_->PlayMove(game.board_->At(4, 1),
                                        game.board_->At(4, 3), &game));
    game.PlayTurn(game.black_->PlayMove(game.board_->At(4, 6),
                                        game.board_->At(4, 4), &game));
    game.PlayTurn(game.white_->PlayMove(game.board_->At(0, 1),
                                        game.board_->At(0, 3), &game));
    game.PlayTurn(game.black_->PlayMove(game.board_->At(3, 7),
                                        game.board_->At(7, 3), &game));
    game.PlayTurn(game.white_->PlayMove(game.board_->At(0, 3),
                                        game.board_->At(0, 4), &game));
    game.PlayTurn(game.black_->PlayMove(game.board_->At(5, 7),
                                        game.board_->At(2, 4), &game));
    game.PlayTurn(game.white_->PlayMove(game.board_->At(0, 4),
                                        game.board_->At(0, 5), &game));
    game.PlayTurn(game.black_->PlayMove(game.board_->At(7, 3),
                                        game.board_->At(5, 1), &game));
    REQUIRE(game.EvaluateBoard() == game::GameState::kBlackWin);
  }
}

TEST_CASE("Test Illegal Game Moves", "[game][illegal]") {
  game::Game game(0);
  SECTION("Test Move To a Square With a Piece of the Same Color") {
    REQUIRE_FALSE(game.PlayTurn(game.white_->PlayMove(game.board_->At(1, 0),
                                        game.board_->At(3, 1), &game)));
  }
  SECTION("Test Move Through Other Pieces") {
    REQUIRE_FALSE(game.PlayTurn(game.white_->PlayMove(game.board_->At(2, 0),
                                        game.board_->At(7, 5), &game)));
    REQUIRE_FALSE(game.PlayTurn(game.white_->PlayMove(game.board_->At(0, 0),
                                        game.board_->At(0, 5), &game)));
    // a knight can jump unlike other pieces
    REQUIRE(game.PlayTurn(game.white_->PlayMove(game.board_->At(1, 0),
                                        game.board_->At(2, 2), &game)));
  }
  SECTION("Test Don't Get Out of Check") {
     game.PlayTurn(game.black_->PlayMove(game.board_->At(4, 6),
                                        game.board_->At(4, 4), &game));
     game.PlayTurn(game.white_->PlayMove(game.board_->At(4, 1),
                                        game.board_->At(4, 3), &game));
     game.PlayTurn(game.black_->PlayMove(game.board_->At(5, 6),
                                        game.board_->At(5, 4), &game));
     game.PlayTurn(game.white_->PlayMove(game.board_->At(3, 0),
                                        game.board_->At(7, 4), &game));
     REQUIRE_FALSE(game.PlayTurn(game.white_->PlayMove(game.board_->At(1, 7),
                                        game.board_->At(0, 4), &game)));
  }
}
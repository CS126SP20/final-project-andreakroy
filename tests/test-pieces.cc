// Copyright (c) 2020 Andrea Roy. All rights reserved.

#include <chess/piece.h>
#include <catch2/catch.hpp>

TEST_CASE("Test Pawn", "[piece][pawn]") {
  piece::Pawn p(piece::Color::kWhite);
  SECTION("Test Horizontal Move") {
    REQUIRE_FALSE(p.CanMove(4, 1, 5, 1));
  }
  SECTION("Test Moving More Than Two Squares") {
    REQUIRE_FALSE(p.CanMove(4, 1, 4, 4));
  }
  SECTION("Test Moving Backwards") {
    REQUIRE_FALSE(p.CanMove(4, 1, 4, 0));
  }
  SECTION("Test Legal Moves") {
    REQUIRE(p.CanMove(2, 1, 2, 3));
    REQUIRE(p.CanMove(6, 1, 6, 2));
    REQUIRE(p.CanMove(5, 4, 4, 5));
  }
  SECTION("Test Two Squares Forward Capture") {
    REQUIRE_FALSE(p.CanMove(5, 4, 4, 6));
  }
}

TEST_CASE("Test Knight", "[piece][knight]") {
  piece::Knight n(piece::Color::kBlack);
  SECTION("Test Legal Moves") {
    REQUIRE(n.CanMove(4, 4, 6, 5));
    REQUIRE(n.CanMove(3, 4, 1, 3));
  }
  SECTION("Test Non Knight Moves") {
    REQUIRE_FALSE(n.CanMove(4, 1, 4, 4));
    REQUIRE_FALSE(n.CanMove(3, 3, 4, 4));
  }
}

TEST_CASE("Test Bishop", "[piece][bishop]") {
  piece::Bishop b(piece::Color::kWhite);
  SECTION("Test Diagonal Moves") {
    REQUIRE(b.CanMove(4, 4, 7, 7));
    REQUIRE(b.CanMove(2, 1, 1, 2));
    REQUIRE(b.CanMove(6, 3, 3, 0));
    REQUIRE(b.CanMove(2, 7, 5, 4));
  }
  SECTION("Test Non Diagonal Moves") {
    REQUIRE_FALSE(b.CanMove(4, 1, 4, 4));
    REQUIRE_FALSE(b.CanMove(3, 3, 2, 6));
  }
}

TEST_CASE("Test Rook", "[piece][rook]") {
  piece::Rook r(piece::Color::kBlack);
  SECTION("Test Vertical Moves") {
    REQUIRE(r.CanMove(4, 6, 4, 2));
    REQUIRE(r.CanMove(1, 0, 1, 7));
  }
  SECTION("Test Horizontal Moves") {
    REQUIRE(r.CanMove(0, 4, 7, 4));
    REQUIRE(r.CanMove(3, 5, 2, 5));
  }
  SECTION("Test Illegal Moves") {
    REQUIRE_FALSE(r.CanMove(4, 1, 7, 6));
    REQUIRE_FALSE(r.CanMove(3, 3, 2, 6));
  }
}

TEST_CASE("Test Queen", "[piece][queen]") {
  piece::Queen q(piece::Color::kBlack);
  SECTION("Test Vertical Moves") {
    REQUIRE(q.CanMove(4, 6, 4, 2));
    REQUIRE(q.CanMove(1, 0, 1, 7));
  }
  SECTION("Test Horizontal Moves") {
    REQUIRE(q.CanMove(0, 4, 7, 4));
    REQUIRE(q.CanMove(3, 5, 2, 5));
  }
  SECTION("Test Diagonal Moves") {
    REQUIRE(q.CanMove(4, 4, 7, 7));
    REQUIRE(q.CanMove(2, 1, 1, 2));
    REQUIRE(q.CanMove(6, 3, 3, 0));
    REQUIRE(q.CanMove(2, 7, 5, 4));
  }
  SECTION("Test Illegal Moves") {
    REQUIRE_FALSE(q.CanMove(4, 1, 7, 6));
    REQUIRE_FALSE(q.CanMove(3, 3, 2, 6));
  }
}

TEST_CASE("Test King", "[piece][king]") {
  piece::King k(piece::Color::kWhite);
  SECTION("Test Moves Within One Square") {
    REQUIRE(k.CanMove(1, 0, 1, 1));
    REQUIRE(k.CanMove(3, 4, 3, 3));
    REQUIRE(k.CanMove(1, 0, 2, 0));
    REQUIRE(k.CanMove(3, 4, 2, 4));
  }
  SECTION("Test One Square Diagonal Moves") {
    REQUIRE(k.CanMove(1, 0, 2, 1));
    REQUIRE(k.CanMove(3, 4, 2, 3));
    REQUIRE(k.CanMove(1, 0, 0, 1));
    REQUIRE(k.CanMove(3, 4, 4, 3));
  }
  SECTION("Test Castling") {
    REQUIRE(k.CanMove(4, 0, 6, 0));
    REQUIRE(k.CanMove(4, 0, 2, 0));
    REQUIRE_FALSE(k.CanMove(4, 7, 6, 7));
    REQUIRE_FALSE(k.CanMove(4, 7, 2, 7));
  }
  SECTION("Test Illegal Moves") {
    REQUIRE_FALSE(k.CanMove(4, 1, 7, 6));
    REQUIRE_FALSE(k.CanMove(3, 3, 2, 6));
  }
}
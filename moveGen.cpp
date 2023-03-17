//
// Created by Thomas Mack on 23/12/2022.
//

#include "moveGen.h"

// TODO optimize a lot (There is a ton to do here but this is an easy start)
void moveGen::legalMoves(const gameState &gs, std::vector<Move> &legalMoves) {
  std::vector<Move> pseudoLegalMoveVector;
  pseudoLegalMoveVector.reserve(16);
  legalMoves.reserve(pseudoLegalMoveVector.size());

  pseudoLegalMoves(gs, pseudoLegalMoveVector);
  /*int checkNum = gs.isKingInCheck(gs.attacking);
  // if king is in check
  if (checkNum) {
      if (checkNum > 1) {
          kingMoves(gs, pseudoLegalMoveVector);
          for (Move m : pseudoLegalMoveVector) {
              gameState gsCopy = gs;
              gsCopy.makeMove(m);
              if (!gsCopy.isKingInCheck(gs.attacking)) legalMoves.push_back(m);
          }
      }
      else {
          // need to generate evasions here
      }
  }
  else {
      // get location of king
      int kingSquare = gs.getPieceBitboard(KING, gs.turn);

      // ignores friendly pieces to look for attacks on the king
      BB maybePinned = Magics::getQueenAttacks(kingSquare, gs.enemyBoard());
      // rough way of doing checks could be to see if 1ULL << fromSquare is on
  the king line. Only then will the move be made and there will be a check to
  see if the king is in check

      pseudoLegalMoves(gs, pseudoLegalMoveVector);

      for (Move move : pseudoLegalMoveVector) {
          gameState gsCopy = gs;
          gsCopy.makeMove(move);
          if (!gsCopy.isKingInCheck(gs.attacking)) {
              legalMoves.push_back(move);
          }
      }
  }*/

  for (Move move : pseudoLegalMoveVector) {
    gameState gsCopy = gs;
    gsCopy.makeMove(move);
    if (!gsCopy.isKingInCheck(gs.getAttacking())) {
      legalMoves.push_back(move);
    }
  }
}

void moveGen::pseudoLegalMoves(const gameState &gs, std::vector<Move> &moves) {
  Color turn = gs.getTurn();

  // TODO optimize passing values and references so we are passing pointers and
  // not copies of values (in some cases passing by value is necessary) we start
  // with leaping piece moves. Pawn, knight, king pawn attacks and moves
  if (turn) {
    moveGen::pawnMoves<BLACK, Quiet>(gs, moves);
    moveGen::pawnMoves<BLACK, Captures>(gs, moves);
    moveGen::pieceMoves<BLACK, QUEEN, All>(gs, moves);
    moveGen::pieceMoves<BLACK, ROOK, All>(gs, moves);
    moveGen::pieceMoves<BLACK, BISHOP, All>(gs, moves);
    moveGen::pieceMoves<BLACK, KNIGHT, All>(gs, moves);
  } else {
    moveGen::pawnMoves<WHITE, Quiet>(gs, moves);
    moveGen::pawnMoves<WHITE, Captures>(gs, moves);
    moveGen::pieceMoves<WHITE, QUEEN, All>(gs, moves);
    moveGen::pieceMoves<WHITE, ROOK, All>(gs, moves);
    moveGen::pieceMoves<WHITE, BISHOP, All>(gs, moves);
    moveGen::pieceMoves<WHITE, KNIGHT, All>(gs, moves);
  }

  kingMoves(gs, moves);
}

void moveGen::kingMoves(const gameState &gs, std::vector<Move> &moves) {
  Color turn = gs.getTurn();
  Color attacking = ~turn;
  BB king = gs.getPieceBitboard(KING, turn);
  BB friendlyPieces = gs.friendlyBoard();
  BB enemyPieces = gs.enemyBoard();
  int kingIndex = Bitboard::getLeastSignificantBit(king);
  BB kingAttacks = Bitboard::kingMoves[kingIndex] ^
                   (Bitboard::kingMoves[kingIndex] & friendlyPieces);

  bool captureFlag = false;
  while (kingAttacks) {
    int kingMoveIndex = pop_lsb(kingAttacks);
    if ((1ULL << kingMoveIndex) & enemyPieces)
      captureFlag = true;
    // bool captureFlag = enemyPieces & (1ULL << kingMoveIndex);
    moves.emplace_back(kingIndex, kingMoveIndex, KING + (6 * turn), false,
                       false, captureFlag, false, false);
    captureFlag = false;
  }
  // castling
  // if it is black's turn
  // todo make sure that the king cannot castle while in check
  if (!gs.isKingInCheck(attacking)) {
    if (turn)
      kingCastling<BLACK>(kingIndex, gs, moves);
    else
      kingCastling<WHITE>(kingIndex, gs, moves);
  }
}

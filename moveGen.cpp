//
// Created by Thomas Mack on 23/12/2022.
//

#include "moveGen.h"

// TODO optimize a lot (There is a ton to do here but this is an easy start)
/*void moveGen::legalMoves(const gameState &gs, std::vector<Move> &legalMoves) {
  std::vector<Move> pseudoLegalMoveVector;
  pseudoLegalMoveVector.reserve(16);
  legalMoves.reserve(pseudoLegalMoveVector.size());

  pseudoLegalMoves(gs, pseudoLegalMoveVector);

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
    kingMoves(gs, moves);
    pawnMoves<BLACK, All>(gs, moves);
    pieceMoves<QUEEN, All>(gs, moves);
    pieceMoves<ROOK, All>(gs, moves);
    pieceMoves<BISHOP, All>(gs, moves);
    pieceMoves<KNIGHT, All>(gs, moves);
    // kingMovesTemplate<BLACK, All>(gs, moves);
  } else {
    kingMoves(gs, moves);
    pawnMoves<WHITE, All>(gs, moves);
    pieceMoves<QUEEN, All>(gs, moves);
    pieceMoves<ROOK, All>(gs, moves);
    pieceMoves<BISHOP, All>(gs, moves);
    pieceMoves<KNIGHT, All>(gs, moves);
    // kingMovesTemplate<WHITE, All>(gs, moves);
  }

  // kingMoves(gs, moves);
}


void moveGen::kingMoves(const gameState &gs, std::vector<Move> &moves) {
  Color turn = gs.getTurn();
  Color attacking = ~turn;
  BB king = gs.getPieceBitboard(KING, turn);
  BB friendlyPieces = gs.friendlyBoard();
  BB enemyPieces = gs.enemyBoard();
  int kingIndex = Bitboard::getLeastSignificantBit(king);
  BB kingAttacks = Bitboard::kingMoves[kingIndex] & ~friendlyPieces;
  // BB kingAttacks = Bitboard::kingMoves[kingIndex] ^
  // (Bitboard::kingMoves[kingIndex] & friendlyPieces);

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
}*/

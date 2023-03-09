//
// Created by Thomas Mack on 02/03/2023.
//

#include "Evaluation.h"

int Evaluation::evaluate(const gameState &gs) {
  int total = 0;
  total += pieceScores(gs);
  total += positionScores(gs);

  return total;
}

int Evaluation::positionScores(const gameState &gs) {
  int totalScore = 0;
  // could add a table that contains references to the piece tables which
  totalScore += getPositionalScoreForPiece(gs.getBitboard(KING).getValue(),
                                           kingTable, WHITE);
  totalScore += getPositionalScoreForPiece(gs.getBitboard(QUEEN).getValue(),
                                           queenTable, WHITE);
  totalScore += getPositionalScoreForPiece(gs.getBitboard(ROOK).getValue(),
                                           rookTable, WHITE);
  totalScore += getPositionalScoreForPiece(gs.getBitboard(BISHOP).getValue(),
                                           bishopTable, WHITE);
  totalScore += getPositionalScoreForPiece(gs.getBitboard(KNIGHT).getValue(),
                                           knightTable, WHITE);
  totalScore += getPositionalScoreForPiece(gs.getBitboard(PAWN).getValue(),
                                           pawnTable, WHITE);

  // black pieces
  totalScore -= getPositionalScoreForPiece(gs.getBitboard(KING).getValue(),
                                           kingTable, BLACK);
  totalScore -= getPositionalScoreForPiece(gs.getBitboard(QUEEN).getValue(),
                                           queenTable, BLACK);
  totalScore -= getPositionalScoreForPiece(gs.getBitboard(ROOK).getValue(),
                                           rookTable, BLACK);
  totalScore -= getPositionalScoreForPiece(gs.getBitboard(BISHOP).getValue(),
                                           bishopTable, BLACK);
  totalScore -= getPositionalScoreForPiece(gs.getBitboard(KNIGHT).getValue(),
                                           knightTable, BLACK);
  totalScore -= getPositionalScoreForPiece(gs.getBitboard(PAWN).getValue(),
                                           pawnTable, BLACK);

  return totalScore;
}

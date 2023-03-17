//
// Created by Thomas Mack on 02/03/2023.
//

#include "Evaluation.h"

int Evaluation::positionScores(const gameState &gs) {
  int totalScore = 0;
  // could add a table that contains references to the piece tables which
  totalScore += getPositionalScoreForPiece(gs.getBitboard(K).getValue(),
                                           kingTable, WHITE);
  totalScore += getPositionalScoreForPiece(gs.getBitboard(Q).getValue(),
                                           queenTable, WHITE);
  totalScore += getPositionalScoreForPiece(gs.getBitboard(R).getValue(),
                                           rookTable, WHITE);
  totalScore += getPositionalScoreForPiece(gs.getBitboard(B).getValue(),
                                           bishopTable, WHITE);
  totalScore += getPositionalScoreForPiece(gs.getBitboard(N).getValue(),
                                           knightTable, WHITE);
  totalScore += getPositionalScoreForPiece(gs.getBitboard(P).getValue(),
                                           pawnTable, WHITE);
  // black pieces

  totalScore -= getPositionalScoreForPiece(gs.getBitboard(k).getValue(),
                                           kingTable, BLACK);
  totalScore -= getPositionalScoreForPiece(gs.getBitboard(q).getValue(),
                                           queenTable, BLACK);
  totalScore -= getPositionalScoreForPiece(gs.getBitboard(r).getValue(),
                                           rookTable, BLACK);
  totalScore -= getPositionalScoreForPiece(gs.getBitboard(b).getValue(),
                                           bishopTable, BLACK);
  totalScore -= getPositionalScoreForPiece(gs.getBitboard(n).getValue(),
                                           knightTable, BLACK);
  totalScore -= getPositionalScoreForPiece(gs.getBitboard(p).getValue(),
                                           pawnTable, BLACK);

  return totalScore;
}

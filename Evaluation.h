//
// Created by Thomas Mack on 02/03/2023.
//

#ifndef CHESS_CPP_EVALUATION_H
#define CHESS_CPP_EVALUATION_H
#include "defsEnums.h"
#include "gameState.h"

class Evaluation {
public:
  static inline int evaluate(const gameState &gs) {
    int total = 0;
    total += pieceScores(gs);
    total += positionScores(gs);

    return (gs.getTurn() == WHITE) ? total : -total;
  }

  static inline int pieceScores(const gameState &gs) {
    int total = 0;
    for (int i = 0; i < pieceCount; i++) {
      BB temp = gs.getBitboard(i).bitCount();
      total += temp * pieceValues[i];
    }
    return total;
  }

  static inline int getPositionalScoreForPiece(BB pieceBitboard,
                                               const int *table, Color turn) {
    int score = 0;
    if (turn) {
      while (pieceBitboard) {
        int index = pop_lsb(pieceBitboard);
        // TODO testing on this to make sure its okay
        score += table[63 - mirrorScores[63 - index]];
      }
    } else {
      while (pieceBitboard) {
        int index = pop_lsb(pieceBitboard);
        score += table[63 - index];
      }
    }
    return score;
  }

  static int positionScores(const gameState &gs);
};

#endif // CHESS_CPP_EVALUATION_H

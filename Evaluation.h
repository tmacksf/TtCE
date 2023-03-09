//
// Created by Thomas Mack on 02/03/2023.
//

#ifndef CHESS_CPP_EVALUATION_H
#define CHESS_CPP_EVALUATION_H
#include "defsEnums.h"
#include "gameState.h"

class Evaluation {
public:
  static int evaluate(const gameState &gs);

  static inline int pieceScores(const gameState &gs) {
    int total = 0;
    for (int i = 0; i < pieceCount; i++) {
      BB temp = gs.getBitboard(i).getValue();
      while (temp) {
        total += pieceValues[i];
        pop_lsb(temp);
      }
    }
    return total;
  }

  static inline int getPositionalScoreForPiece(BB pieceBitboard,
                                               const int *table, Color turn) {
    int score = 0;
    if (turn) {
      while (pieceBitboard) {
        int index = pop_lsb(pieceBitboard);
        score += table[mirrorScores[index]];
      }
    } else {
      while (pieceBitboard) {
        int index = pop_lsb(pieceBitboard);
        score += table[index];
      }
    }
    return 0;
  }

  static int positionScores(const gameState &gs);
};

#endif // CHESS_CPP_EVALUATION_H

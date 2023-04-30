//
// Created by Thomas Mack on 02/03/2023.
//

#ifndef CHESS_CPP_EVALUATION_H
#define CHESS_CPP_EVALUATION_H
#include "defsEnums.hpp"
#include "gameState.hpp"

namespace Eval {

class Evaluation {
public:
  static inline int evaluate(const gameState &gs) {
    int phaseScore = getGamePhase(gs);

    Phase phase;
    // get current phase
    if (phaseScore > OPENINGSCORE) {
      phase = Opening;
    } else if (phaseScore < ENDGAMESCORE) {
      phase = Endgame;
    } else {
      phase = Middlegame;
    }

    int total = 0;
    total += pieceScores(gs, phase, phaseScore);
    if (phase == Middlegame)
      total += positionScores<Middlegame>(gs, phaseScore);
    else if (phase == Opening)
      total += positionScores<Opening>(gs, phaseScore);
    else
      total += positionScores<Endgame>(gs, phaseScore);

    return (gs.getTurn() == WHITE) ? total : -total;
  }

  static inline int pieceScores(const gameState &gs, Phase gamePhase,
                                int phaseScore) {
    int total = 0;
    for (int i = 0; i < pieceCount; i++) {
      int pieceCount = gs.getPieceCount(i);

      if (gamePhase == Opening || gamePhase == Endgame) {
        total += materialScore[gamePhase][i] * pieceCount;
      } else {
        int oldTotal = total;
        total += ((materialScore[Opening][i] * phaseScore +
                   materialScore[Endgame][i] * (OPENINGSCORE - ENDGAMESCORE)) /
                  OPENINGSCORE) *
                 pieceCount;
      }
    }
    return total;
  }

  template <Phase gamePhase>
  static inline int positionScores(const gameState &gs, int phaseScore) {
    int total = 0;
    BB pieceBitboard;

    // white pieces
    for (int i = K; i < k; i++) {
      pieceBitboard = gs.getBitboard(i).getValue();

      while (pieceBitboard) {
        int location = pop_lsb(pieceBitboard);
        total += scoreForSquare<gamePhase, WHITE>(i, location, phaseScore);
      }
    }

    // black pieces
    for (int i = K; i < k; i++) {
      pieceBitboard = gs.getBitboard(i + 6).getValue();
      while (pieceBitboard) {
        int location = pop_lsb(pieceBitboard);
        total -= scoreForSquare<gamePhase, BLACK>(i, location, phaseScore);
      }
    }
    return total;
  }

  // Gets score for a position from the piece tables based on game phase and
  // turn
  template <Phase gamePhase, Color Turn>
  static inline int scoreForSquare(int piece, int loc, int phaseScore) {
    int location = (Turn == BLACK ? mirrorScores[loc] : 63 - loc);
    int score = 0;

    if constexpr (gamePhase == Middlegame) {
      score += ((positionalScores[Opening][piece][location] * phaseScore +
                 positionalScores[Endgame][piece][location] *
                     (OPENINGSCORE - ENDGAMESCORE)) /
                OPENINGSCORE);
    }
    if constexpr (gamePhase == Endgame || gamePhase == Opening) {
      score += positionalScores[gamePhase][piece][location];
    }
    return score;
  }

  // gets the current game phase for evaluation
  static inline int getGamePhase(const gameState &gs) {
    int whitePieces = 0;
    int blackPieces = 0;

    for (int i = Q; i < P; i++) {
      whitePieces += gs.getPieceCount(i) * materialScore[Opening][i];
    }
    for (int i = q; i < p; i++) {
      blackPieces += gs.getPieceCount(i) * materialScore[Opening][i];
    }
    return whitePieces - blackPieces;
  }

  // NNUE Evaluation

  // Changes this engines piece encoding to stockfish's NNUE
  static constexpr int NNUEPieceNums[] = {0, 0, 0, 0, 0, 0, 1};

  static inline int NNUEInput(const gameState &gs, int *pieces, int *squares) {
    return 0;
  }

  static inline int stockfishNNUE(const gameState &gs) {
    int pieces[33];
    int positions[33];

    return 0;
  }
};

} // namespace Eval

#endif // CHESS_CPP_EVALUATION_H

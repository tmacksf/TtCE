//
// Created by Thomas Mack on 02/03/2023.
//

#ifndef TTCE_EVALUATION_HPP
#define TTCE_EVALUATION_HPP

#include "defsEnums.hpp"
#include "gameState.hpp"

namespace TtCE {
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
    long opening = 0;
    long endgame = 0;
    if (phase == Middlegame) {
      opening += pieceScores<Opening>(gs);
      opening += positionScores<Opening>(gs);

      endgame += pieceScores<Endgame>(gs);
      endgame += positionScores<Endgame>(gs);

      total = (opening * phaseScore + endgame * (OPENINGSCORE - phaseScore)) /
              OPENINGSCORE;
    }

    else if (phase == Opening) {
      total += positionScores<Opening>(gs);
      total += pieceScores<Opening>(gs);
    }

    else {
      total += positionScores<Endgame>(gs);
      total += pieceScores<Endgame>(gs);
    }

    return (gs.getTurn() == WHITE) ? total : -total;
  }

  template <Phase phase> static inline long pieceScores(const gameState &gs) {
    long total = 0;
    for (int i = 0; i < pieceCount; i++) {
      total += materialScore[phase][i] * gs.getBitboard(i).bitCount();
    }
    return total;
  }

  template <Phase gamePhase>
  static inline int positionScores(const gameState &gs) {
    int total = 0;
    BB pieceBitboard;

    for (int i = 0; i < pieceCount / 2; i++) {
      // white pieces
      pieceBitboard = gs.getBitboard(i).getValue();

      while (pieceBitboard) {
        int location = 63 - pop_lsb(pieceBitboard);
        total += positionalScores[gamePhase][i][location];
      }

      // black pieces
      pieceBitboard = gs.getBitboard(i + 6).getValue();
      while (pieceBitboard) {
        int location = mirrorScores[pop_lsb(pieceBitboard)];
        total -= positionalScores[gamePhase][i][location];
      }
    }
    return total;
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
} // namespace TtCE
#endif // TTCE_EVALUATION_HPP

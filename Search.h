//
// Created by Thomas Mack on 08/03/2023.
//

#ifndef CHESS_CPP_SEARCH_H
#define CHESS_CPP_SEARCH_H

#include "Evaluation.h"
#include "moveGen.h"

class Search {
private:
  int m_nodes;
  Move m_bestMove;
  int m_alpha;
  int m_beta;
  int m_maxDepth;

public:
  Search() : m_bestMove(-1) {
    m_nodes = 0;
    m_alpha = -50000;
    m_beta = 50000;
    m_maxDepth = 0;
  }

  Move &getBestMove() { return m_bestMove; }
  int getNodes() { return m_nodes; }

  int negamax(const gameState &gs, int alpha, int beta, int depth) {
    m_nodes++;

    if (depth == 0) {
      return Evaluation::evaluate(gs);
    }

    std::vector<Move> moves;
    moves.reserve(32);
    moveGen::legalMoves(gs, moves);

    int oldAlpha = alpha;
    Move bestSoFar{-1};

    for (Move m : moves) {
      gameState gsCopy = gs;
      gsCopy.makeMove(m);
      int score = -negamax(gsCopy, -beta, -alpha, depth - 1);

      // fail hard beta cutoff
      if (score >= beta) {
        return beta; // move fails high
      }

      if (score > alpha) {
        // PV node and where we associate the best move with the best score
        alpha = score;

        if (depth == m_maxDepth) {
          bestSoFar = m;
        }
      }
    }

    if (oldAlpha != alpha) {
      m_bestMove = bestSoFar;
    }

    // move fails low
    return alpha;
    // not sure if i have to delete vector
  }

  int findBestMove(gameState gs, int depth) {
    m_maxDepth = depth;
    int score = negamax(gs, m_alpha, m_beta, depth);
    return score;
  }
};

#endif // CHESS_CPP_SEARCH_H

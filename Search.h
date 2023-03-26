//
// Created by Thomas Mack on 08/03/2023.
//

#ifndef CHESS_CPP_SEARCH_H
#define CHESS_CPP_SEARCH_H

#include "Evaluation.h"
#include "defsEnums.h"
#include "moveGen.h"

class Search {
private:
  int m_nodes;
  Move m_bestMove;
  int m_alpha;
  int m_beta;
  int m_maxDepth;
  int m_qNodes;
  int m_maxQDepth;

public:
  Search() : m_bestMove(-1) {
    m_nodes = 0;
    m_alpha = -50000;
    m_beta = 50000;
    m_maxDepth = 0;
    m_qNodes = 0;
    m_maxQDepth = 2;
  }

  Move &getBestMove() { return m_bestMove; }
  int getNodes() { return m_nodes; }
  int getQNodes() { return m_qNodes; }

  int quiescence(const gameState &gs, int alpha, int beta, int depth) {
    if (!depth)
      return Evaluation::evaluate(gs);

    int eval = Evaluation::evaluate(gs);
    m_qNodes++;

    if (eval >= beta) {
      return beta;
    }
    if (alpha < eval) {
      alpha = eval;
    }

    std::vector<Move> moves;
    moves.reserve(8);
    moveGen::legalMoves<Captures>(gs, moves);

    for (Move &m : moves) {
      gameState gsCopy = gs;
      gsCopy.makeMove(m);
      int score = -quiescence(gsCopy, -beta, -alpha, depth - 1);
      if (score >= beta) {
        return beta;
      }
      if (score > alpha) {
        alpha = score;
      }
    }

    return alpha;
  }

  int negamax(const gameState &gs, int alpha, int beta, int depth) {
    m_nodes++;

    if (depth == 0) {
      return quiescence(gs, alpha, beta, m_maxQDepth);
    }

    std::vector<Move> moves;
    moves.reserve(32);
    moveGen::legalMoves<All>(gs, moves);

    int oldAlpha = alpha;
    Move bestSoFar{-1};

    bool isKingInCheck = gs.isKingInCheck(gs.getAttacking());

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
          m.printMove();
          std::cout << "Score: " << score << std::endl;
          bestSoFar = m;
        }
      }
    }

    if (!moves.size()) {
      if (gs.isKingInCheck(gs.getAttacking()))
        return -49000 + m_maxDepth - depth;
      // not sure if I need to have this but instead need to have a ply (half
      // move counter)
      else
        return 0; // draw
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

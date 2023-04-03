//
// Created by Thomas Mack on 08/03/2023.
//

#ifndef CHESS_CPP_SEARCH_H
#define CHESS_CPP_SEARCH_H

#include "Evaluation.h"
#include "defsEnums.h"
#include "moveGen.h"
#include <random>

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
  // zobrist information

  static BB zArray[12][64];
  static BB zEP[8];
  static BB zCastle[4];
  static BB zBlackMove;

  Search() : m_bestMove() {
    m_nodes = 0;
    m_alpha = -50000;
    m_beta = 50000;
    m_maxDepth = 0;
    m_qNodes = 0;
    m_maxQDepth = 4;
  }

  Move &getBestMove() { return m_bestMove; }
  int getNodes() { return m_nodes; }
  int getQNodes() { return m_qNodes; }

  int quiescence(gameState &gs, int alpha, int beta, int depth) {
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
    moveGen::sortMoves(gs, moves);

    for (Move &m : moves) {
      gameState gsCopy = gs;
      gsCopy.makeMove(m);
      int score = -quiescence(gsCopy, -beta, -alpha, depth - 1);
      if (score >= beta) {
        Move::killerMove[1][gs.getPly()] = Move::killerMove[0][gs.getPly()];
        Move::killerMove[0][gs.getPly()] = m;
        return beta;
      }
      if (score > alpha) {
        Move::historyMove[m.m_piece][m.m_toSquare] += depth;
        alpha = score;
      }
    }

    return alpha;
  }

  int negamax(gameState &gs, int alpha, int beta, int depth) {
    m_nodes++;

    if (depth == 0) {
      return quiescence(gs, alpha, beta, m_maxQDepth);
    }
    // TODO: add escape for when MAX_DEPTH is reached

    std::vector<Move> moves;
    moves.reserve(32);
    moveGen::legalMoves<All>(gs, moves);
    moveGen::sortMoves(gs, moves);

    int oldAlpha = alpha;
    Move bestSoFar; // holder

    bool isKingInCheck = gs.isKingInCheck(gs.getAttacking());

    for (Move &m : moves) {
      gameState gsCopy = gs;
      gsCopy.makeMove(m);
      int score = -negamax(gsCopy, -beta, -alpha, depth - 1);

      // fail hard beta cutoff
      if (score >= beta) {
        if (!m.m_captureFlag) {
          Move::killerMove[1][gs.getPly()] = Move::killerMove[0][gs.getPly()];
          Move::killerMove[0][gs.getPly()] = m;
        }
        return beta; // move fails high
      }

      if (score > alpha) {
        // PV node and where we associate the best move with the best score

        Move::historyMove[m.m_piece][m.m_toSquare] += depth;
        alpha = score;

        if (depth == m_maxDepth) {
          m.printMove();
          std::cout << " Score: " << score << std::endl;
          bestSoFar = m;
        }
      }
    }

    if (!moves.size()) {
      if (gs.isKingInCheck(gs.getAttacking()))
        return -49000 + m_maxDepth - depth;
      else
        return 0; // draw
    }

    if (oldAlpha != alpha) {
      m_bestMove = bestSoFar;
    }

    // fails low
    return alpha;
  }

  int findBestMove(gameState gs, int depth) {
    m_maxDepth = depth;
    int score = negamax(gs, m_alpha, m_beta, depth);
    return score;
  }

  // Zobrist hashing
  static void initZobrist();
  static BB getHash(const gameState &gs);
};

#endif // CHESS_CPP_SEARCH_H

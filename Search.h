//
// Created by Thomas Mack on 08/03/2023.
//

#ifndef CHESS_CPP_SEARCH_H
#define CHESS_CPP_SEARCH_H

#include "Evaluation.h"

class Search {
private:
  int m_alpha;
  int m_beta;
  int m_nodes;

public:
  Search(int alpha, int beta) {
    m_alpha = alpha;
    m_beta = beta;
  }
  // boilerplate
  int getAlpha() { return m_alpha; }
  int getBeta() { return m_beta; }
  int getNodes() { return m_nodes; }

  void setAlpha(int alpha) { m_alpha = alpha; }
  void setBeta(int beta) { m_beta = beta; }

  int inline negamax(const gameState &gs, int depth) {

    if (depth == 0) {
      return Evaluation::evaluate(gs);
    }

    m_nodes++;

    return negamax(gs, depth - 1);
  }
};

#endif // CHESS_CPP_SEARCH_H

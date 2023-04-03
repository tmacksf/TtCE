//
// Created by Thomas Mack on 04/02/2023.
//

#ifndef CHESS_CPP_TESTING_H
#define CHESS_CPP_TESTING_H

#include "game.h"

class Testing {
public:
  static int quickTests();
  static int AllTests();
  static BB Perft(gameState &gs, int depth, int debugging);
  static void perftTest(gameState &gs, int perftDepth, int debug);
  static BB PerftAdvanced(gameState &gs, int depth, int *extraInfo);

  static BB advancedPerftDriver(gameState &gs, int depth);
};

#endif // CHESS_CPP_TESTING_H

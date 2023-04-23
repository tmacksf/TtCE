//
// Created by Thomas Mack on 04/02/2023.
//

#ifndef CHESS_CPP_TESTING_H
#define CHESS_CPP_TESTING_H

#include "UCI.h"

class Testing {
public:
  static int quickTests();
  static int AllTests();
  static BB Perft(gameState &gs, int depth);
  static void perftTest(gameState &gs, int perftDepth);
  static BB PerftAdvanced(gameState &gs, int depth, int *extraInfo);
  static void allPerftTests();
  static BB PerftBrokenUp(gameState &gs, int depth);

  static BB advancedPerftDriver(gameState &gs, int depth);

  static int hashTesting(gameState &gs, int depth, BB hash);
};

#endif // CHESS_CPP_TESTING_H

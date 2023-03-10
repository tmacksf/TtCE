//
// Created by Thomas Mack on 04/02/2023.
//

#ifndef CHESS_CPP_TESTING_H
#define CHESS_CPP_TESTING_H

#include "game.h"

class Testing {
public:
    static int AllTests();
    static BB Perft(const gameState &gs, int depth);
    static void perftTest(const gameState &gs, int perftDepth);
    static BB PerftAdvanced(const gameState &gs, int depth, int *extraInfo);

    static BB advancedPerftDriver(gameState &gs, int depth);
};


#endif //CHESS_CPP_TESTING_H

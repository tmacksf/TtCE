//
// Created by Thomas Mack on 04/02/2023.
//

#ifndef CHESS_CPP_TESTING_H
#define CHESS_CPP_TESTING_H

#include "game.h"

class Testing {
public:
    Testing() =default;

    int Perft(const gameState &gs, int depth);
};


#endif //CHESS_CPP_TESTING_H

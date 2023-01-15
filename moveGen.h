//
// Created by Thomas Mack on 23/12/2022.
//

#ifndef CHESS_CPP_MOVEGEN_H
#define CHESS_CPP_MOVEGEN_H
#include "gameState.h"

class moveGen {
public:
    moveGen();
    std::vector<gameState> *legalMoves(gameState &gc);
    void pseudoLegalMoves(gameState gc, std::vector<Move> *moves);

};
#endif //CHESS_CPP_MOVEGEN_H

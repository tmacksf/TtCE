//
// Created by Thomas Mack on 23/12/2022.
//

#ifndef CHESS_CPP_MOVEGEN_H
#define CHESS_CPP_MOVEGEN_H
#include "gameState.h"

class moveGen {
public:
    moveGen()= default;
    static void legalMoves(gameState gs, std::vector<Move> *moves);
    static void pseudoLegalMoves(gameState gs, std::vector<Move> *moves);
    // might want to make these inline to reduce function call overhead
    static void pawnMoves(gameState gs, std::vector<Move> *moves);
};
#endif //CHESS_CPP_MOVEGEN_H

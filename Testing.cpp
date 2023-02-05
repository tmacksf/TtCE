//
// Created by Thomas Mack on 04/02/2023.
//

#include "Testing.h"

int Testing::Perft(const gameState &gs, int depth) {
    std::vector<Move> moves;
    moves.reserve(16);
    moveGen::legalMoves(gs, moves);

    std::cout << moves.size() << std::endl;

    BB nodes = 0;
    for (Move move : moves){
        gameState newGs = gs;
        newGs.makeMove(move);
        nodes += Perft(newGs, depth - 1);
    }

    return nodes;
}

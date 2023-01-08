//
// Created by Thomas Mack on 23/12/2022.
//

#include "moveGen.h"

void pseudoLegalMoves(gameState gc) {
    gameState gcClone = gc;
    std::vector<gameState>();
    int turn = gcClone.turn;
    BB allPieces = gcClone.allPieces();
    BB friendlyPieces = gcClone.friendlyBoard();
    BB enemyPieces = gcClone.enemyBoard();
    for (int i = 0; i < 6; i ++) {

    }
}
//
// Created by Thomas Mack on 23/12/2022.
//

#include "game.h"

using namespace std;

void initializeAll() {
    Bitboard::initAttackTables();
    Magics::generateSlidingAttackTables();
}

int Game(){
    gameState gs{};
    // need to do these two things before the game starts so need to make these a function and have it run at beginning
    initializeAll();
    gs.initialise(STARTING_FEN);
    //gs.initialise("rnb1kbnr/pppp1ppp/4q3/8/8/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1");
    //gs.initialise("4k3/8/8/4p3/2p5/1B4B1/8/R3K3 w Q - 0 1");


    vector<Move> moveVec;
    moveVec.reserve(32);
    moveGen::legalMoves(gs, moveVec);
    gs.makeMove(moveVec[0]);
    cout << "Turn: " << gs.turn << " Attacking: "<< gs.attacking << endl;
    vector<Move> nextMoves;
    nextMoves.reserve(32);

    auto start = chrono::steady_clock::now();
    moveGen::legalMoves(gs, nextMoves);
    auto end = chrono::steady_clock::now();
    auto diff = end - start;
    cout << chrono::duration <double, nano> (diff).count() << " ns" << endl;
    cout << moveVec.size() << endl;
    for (Move b : nextMoves) {
        b.printMove();
    }

    return 0;
}

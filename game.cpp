//
// Created by Thomas Mack on 23/12/2022.
//

#include "moveGen.h"

using namespace std;

void initializeAll() {
    Bitboard::initAttackTables();
    Magics::generateSlidingAttackTables();
}

int Game(){
    gameState gs{};
    // need to do these two things before the game starts so need to make these a function and have it run at beginning
    initializeAll();
    //gs.initialise(STARTING_FEN);
    gs.initialise("4k3/8/8/4p3/2p5/1B4B1/8/R3K3 w Q - 0 1");

    //gs.bitboards[11].setBitAt(17);
    vector<Move> moveVec;
    moveVec.reserve(16);
    auto start = chrono::steady_clock::now();
    moveGen::pseudoLegalMoves(gs, moveVec);
    auto end = chrono::steady_clock::now();
    auto diff = end - start;
    cout << chrono::duration <double, nano> (diff).count() << " ns" << endl;
    cout << moveVec.size() << endl;
    for (Move b : moveVec) {
        cout << "Piece :" << b.piece << " Capture: " << b.captureFlag << " Double push: " << b.doublePushFlag << " From: " << b.fromSquare << " To: " << b.toSquare << endl;
    }

    return 0;
}

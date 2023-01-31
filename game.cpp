//
// Created by Thomas Mack on 23/12/2022.
//

#include "moveGen.h"

using namespace std;

int Game(){
    Piece pcs{};
    gameState gs{};
    Bitboard::initAttackTables();
    Magics::generateSlidingAttackTables();
    gs.initialise(STARTING_FEN);

    /*//gs.bitboards[11].setBitAt(17);
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
    }*/

    return 0;
}

//
// Created by Thomas Mack on 23/12/2022.
//

#include "moveGen.h"

using namespace std;

int Game(){
    Piece pcs{};
    gameState gs{};
    Bitboard::initAttackTables();
    BB testing = 0xFF00ULL;
    gs.initialise(STARTING_FEN);

    /*while (testing) {
        int shift = Bitboard::getLeastSignificantBit(testing);
        testing = testing >> shift;
        cout << bitset<64>(testing) << endl;

        unset_bit(testing, 0);
    }*/

    gs.bitboards[11].setBitAt(17);
    vector<Move> moveVec;
    moveGen::pseudoLegalMoves(gs, &moveVec);
    cout << moveVec.size() << endl;
    for (Move b : moveVec) {
        cout << "Piece :" << b.piece << " Capture: " << b.captureFlag << " Double push: " << b.doublePushFlag << " From: " << b.fromSquare << " To: " << b.toSquare << endl;
    }

    return 0;
}

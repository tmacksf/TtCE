//
// Created by Thomas Mack on 23/12/2022.
//

#include "moveGen.h"

using namespace std;

int Game(){
    Piece pcs{};
    gameState gs{};
    Bitboard::initAttackTables();
    gs.initialise(STARTING_FEN);
    for (int i = 0; i < 12; i++) {
        cout << i << " " <<gs.bitboards[i].bitCount() << endl;
        gs.bitboards[i].printBitboard();
    }
    gs.printing(pcs);

    return 0;
}

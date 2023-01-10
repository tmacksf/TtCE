//
// Created by Thomas Mack on 23/12/2022.
//

#include "moveGen.h"

using namespace std;

int Game(){
    Piece pcs{};
    gameState gs{};
    Bitboard file{};
    file.initializePawnAttacks();
    //file.printBitboard();
    Bitboard bb{};
    //cout << bitset<64>(gs.bitboards[0].getValue()) << endl;
    return 0;
}

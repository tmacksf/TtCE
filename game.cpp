//
// Created by Thomas Mack on 23/12/2022.
//

#include <iostream>
#include <bitset>
#include "moveGen.h"

using namespace std;

int Game(){
    Piece pcs{};
    Bitboard bb{0xFFFFFFFFFFFFFFF0Ull};
    gameState gs;
    cout << std::bitset<64>(gs.bitboards[0].getValue()) << endl;
    gs.initialise(STARTING_FEN);
    gs.printing(pcs);
    cout << std::bitset<64>(gs.bitboards[0].getValue()) << endl;
    cout << bb.getBitAt(63) << endl;
    bb.setBitAt(0);
    return 0;
}

//
// Created by Thomas Mack on 23/12/2022.
//

#include <iostream>
#include <bitset>
#include "moveGen.h"

using namespace std;

int main(){

    BB wKing = 0x0000000000000000ULL;
    BB bKing = 0x0000000000000000ULL;
    BB wQueens = 0x0000000000000000ULL;
    BB bQueens = 0x0000000000000000ULL;
    BB wRooks = 0x0000000000000000ULL;
    BB bRooks = 0x0000000000000000ULL;
    BB wBishops = 0x0000000000000000ULL;
    BB bBishops = 0x0000000000000000ULL;
    BB wKnights = 0x0000000000000000ULL;
    BB bKnights = 0x0000000000000000ULL;
    BB wPawns = 0x0000000000000000ULL;
    BB bPawns = 0x0000000000000000ULL;

    //gameState *gc = new gameState(wKing, bKing, wQueens, bQueens, wRooks, bRooks,
    //                              wBishops, bBishops, wKnights, bKnights, wPawns, bPawns);

    Bitboard bb{0xFFFFFFFFFFFFFFF0Ull};

    cout << std::bitset<64>(bb.getValue()) << endl;
    cout << bb.getBitAt(63) << endl;
    bb.setBitAt(0);
    return 0;
}

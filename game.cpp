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

    while (testing) {
        int shift = Bitboard::getLeastSignificantBit(testing);
        testing = testing >> shift;
        cout << bitset<64>(testing) << endl;

        unset_bit(testing, 0);
    }
    return 0;
}

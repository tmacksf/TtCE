//
// Created by Thomas Mack on 03/01/2023.
//

#include "Bitboard.h"

int Bitboard::getBitAt(int index) {
    assert(0 <= index < 64);
    return (this->b & (1ULL << index)) ? 1 : 0;
}

void Bitboard::setBitAt(int index) {
    assert(0 <= index < 64);
    this->b |= 1ULL << index;
}

void Bitboard::unSetBitAt(int index) {
    assert(0 <= index < 64);
    if (getBitAt(index) == 1ULL) {
        toggleBit(index);
    }
}

void Bitboard::toggleBit(int index) {
    assert(0 <= index < 64);
    this->b ^= 1ULL << index;
}

int Bitboard::pieceCount(){
    int total = 0;
    for (int i = 0; i < 64; i++) {
        total += getBitAt(i);
    }
    return total;
}
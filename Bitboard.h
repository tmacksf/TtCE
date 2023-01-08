//
// Created by Thomas Mack on 23/12/2022.
//
#include "defsEnums.h"

#ifndef CHESS_CPP_BITBOARDS_H
#define CHESS_CPP_BITBOARDS_H

class Bitboard {
private:
    BB b;

public:
    // map of pieces and their ascii chars

    // constructors
    Bitboard(){
        this->b = 0ULL;
    }
    Bitboard(BB b){
        this->b = b;
    }

    BB getValue() const{
        return this->b;
    }

    // bitwise methods
    int getBitAt(int index);
    void setBitAt(int index);
    void unSetBitAt(int index);
    void toggleBit(int index);
    int pieceCount();
};

#endif //CHESS_CPP_BITBOARDS_H

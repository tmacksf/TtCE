//
// Created by Thomas Mack on 23/12/2022.
//
#include "defsEnums.h"

#ifndef CHESS_CPP_BITBOARDS_H
#define CHESS_CPP_BITBOARDS_H

class Bitboard {
private:
    BB m_bitboard;

public:

    // constructors
    Bitboard(){
        this->m_bitboard = 0ULL;
    }
    Bitboard(BB b) : m_bitboard(b)
    {}

    // getter method
    BB getValue() const{
        return this->m_bitboard;
    }

    // bitwise methods
    int getBitAt(int index) const;
    void setBitAt(int index);
    void unSetBitAt(int index);
    static BB inline unSetBitAt(BB bBoard, int index) {
        return (bBoard &= ~(1ULL << index));
    }
    void toggleBit(int index);
    int getLeastSignificantBit() const;
    static int inline getLeastSignificantBit(BB b) {
        return __builtin_ctzll(b);
    }
    int bitCount() const;
    // overloaded bit count to take a bitboard as an argument. Done using another method. Will test the speed of both of these later on
    static inline int bitCount(BB x) {
        x -= (x >> 1) & 0x5555555555555555; //put count of each 2 bits into those 2 bits
        x = (x & 0x3333333333333333) + ((x >> 2) & 0x3333333333333333); //put count of each 4 bits into those 4 bits
        x = (x + (x >> 4)) & 0x0f0f0f0f0f0f0f0f;  //put count of each 8 bits into those 8 bits
        return ((x * 0x0101010101010101) >> 56);
    }
    // pretty printing of the bitboard
    void printBitboard() const;

    // static attack tables to do speedy move generation all but the pawn ones are the same for both colors
    static BB kingMoves[64];
    static BB knightMoves[64];
    static BB pawnMoves[2][64];
    static BB pawnAttacks[2][64];
    //rook and bishop attack tables are initialized in the magics section because they are done magically

    // methods to initialize attack tables
    static void initAttackTables();
    static void initPawnAttacks();
    static void initPawnMoves();
    static void initKnightAttacks();

    static void initKingAttacks();

    // need to add way to generate on the fly

    // making it easier to do generation of moves by segmenting the generation of attack
    static BB rayAttack(Direction direction, int index, int depth);
};

#endif //CHESS_CPP_BITBOARDS_H

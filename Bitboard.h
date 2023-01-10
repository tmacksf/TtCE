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
    // static

    // constructors
    Bitboard(){
        this->m_bitboard = 0ULL;
    }
    Bitboard(BB b){
        this->m_bitboard = b;
    }

    // getter method
    BB getValue() const{
        return this->m_bitboard;
    }

    // bitwise methods
    int getBitAt(int index);
    void setBitAt(int index);
    void unSetBitAt(int index);
    void toggleBit(int index);
    int pieceCount();
    // pretty printing of the bitboard
    void printBitboard();

    // static attack tables to do speedy move generation all but the pawn ones are the same for both colors
    static BB kingMoves[64];
    static BB queenMoves[64];
    static BB rookMoves[64];
    static BB bishopMoves[64];
    static BB knightMoves[64];
    static BB pawnMoves[2][64];
    static BB whitePawnMoves[64];
    static BB blackPawnMoves[64];

    // method to initialize attack tables
    static void initializeAttackTables();
    static void initializePawnAttacks();
    static void initializeKnightAttacks();
    static void initializeBishopAttacks();
    static void initializeRookAttacks();
    static void initializeQueenAttacks();
    static void initializeKingAttacks();
};

#endif //CHESS_CPP_BITBOARDS_H

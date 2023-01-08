//
// Created by Thomas Mack on 23/12/2022.
//
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <cassert>
#include <vector>

#ifndef CHESS_CPP_BITBOARDS_H
#define CHESS_CPP_BITBOARDS_H

#define STARTING_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
typedef uint64_t BB;

// all files, ranks, etc
const BB aFile = 0x0ULL;
const BB bFile = 0x0ULL;
const BB cFile = 0x0ULL;
const BB dFile = 0x0ULL;
const BB eFile = 0x0ULL;
const BB fFile = 0x0ULL;
const BB gFile = 0x0ULL;
const BB hFile = 0x0ULL;

const BB rank1 = 0x0ULL;
const BB rank2 = 0x0ULL;
const BB rank3 = 0x0ULL;
const BB rank4 = 0x0ULL;
const BB rank5 = 0x0ULL;
const BB rank6 = 0x0ULL;
const BB rank7 = 0x0ULL;
const BB rank8 = 0x0ULL;

// defining enums for directions and pieces
enum Color : int {
    WHITE,
    BLACK,
    NONE
};

enum Castling : int {
    whiteQueen,
    whiteKing,
    blackQueen,
    blackKing
};

enum Board : int {
    H1, G1, F1, E1, D1, C1, B1, A1,
    H2, G2, F2, E2, D2, C2, B2, A2,
    H3, G3, F3, E3, D3, C3, B3, A3,
    H4, G4, F4, E4, D4, C4, B4, A4,
    H5, G5, F5, E5, D5, C5, B5, A5,
    H6, G6, F6, E6, D6, C6, B6, A6,
    H7, G7, F7, E7, D7, C7, B7, A7,
    H8, G8, F8, E8, D8, C8, B8, A8,
};

enum Direction : int {
    NORTH = 8,
    SOUTH = -NORTH,
    EAST = 1,
    WEST = -EAST,
    NORTH_EAST = NORTH + EAST,
    NORTH_WEST = NORTH + WEST,
    SOUTH_EAST = SOUTH + EAST,
    SOUTH_WEST = SOUTH + WEST
};

enum PieceTypes : int {
    KING,
    QUEEN,
    ROOK,
    BISHOP,
    KNIGHT,
    PAWN
};

enum Pieces : int {
    K,
    k,
    Q,
    q,
    R,
    r,
    N,
    n,
    B,
    b,
    P,
    p
};

class Bitboard {
private:
    BB b;

public:
    Bitboard(){
        this->b = 0ULL;
    }

    Bitboard(BB b){
        this->b = b;
    }

    BB getValue() const{
        return this->b;
    }

    int getBitAt(int index);
    void setBitAt(int index);
    void unSetBitAt(int index);
    void toggleBit(int index);
    int pieceCount();
};

#endif //CHESS_CPP_BITBOARDS_H

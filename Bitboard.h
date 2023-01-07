//
// Created by Thomas Mack on 23/12/2022.
//
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <iostream>

#ifndef CHESS_CPP_BITBOARDS_H
#define CHESS_CPP_BITBOARDS_H

typedef uint64_t BB;

// all files, ranks, etc
const BB aFile = 0x0ULL;

// defining enums for directions and pieces
enum Color : int {
    WHITE,
    BLACK,
    NONE
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
    ALL = 0,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};

enum Pieces : int {
    P = PAWN,
    N,
    B,
    R,
    Q,
    K,
    p = P + 6,
    n,
    b,
    r,
    q,
    k
};

class Bitboard {
private:
    BB b;

public:
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

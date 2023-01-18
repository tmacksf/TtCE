//
// Created by Tommy on 1/8/2023.
//

#ifndef CHESS_CPP_DEFSENUMS_H
#define CHESS_CPP_DEFSENUMS_H

#include <cstdint>
#include <cstdlib>
#include <string>
#include <iostream>
#include <cassert>
#include <vector>
#include <unordered_map>
#include <bitset>
#include <sstream>

#define STARTING_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
typedef uint64_t BB;

#define set_bit(bBoard, index) (bBoard |= 1ULL << index)
#define unset_bit(bBoard, index) (bBoard &= ~(1ULL << index))
#define get_bit(bBoard, index) ((bBoard & 1ULL << index) ? 1 : 0)

// all files, ranks, etc
const BB hFile = 0x0101010101010101ULL;
const BB gFile = hFile << 1;
const BB fFile = hFile << 2;
const BB eFile = hFile << 3;
const BB dFile = hFile << 4;
const BB cFile = hFile << 5;
const BB bFile = hFile << 6;
const BB aFile = hFile << 7;
const BB files[] = {hFile, gFile, fFile, eFile, dFile, cFile, bFile, aFile};

const BB rank8 = 0xFF00000000000000ULL;
const BB rank7 = rank8 >> 8;
const BB rank6 = rank7 >> 8;
const BB rank5 = rank6 >> 8;
const BB rank4 = rank5 >> 8;
const BB rank3 = rank4 >> 8;
const BB rank2 = rank3 >> 8;
const BB rank1 = rank2 >> 8;
const BB ranks[] = {rank1, rank2, rank3, rank4, rank5, rank6, rank7, rank8};

// diagonals
const BB rightLeft = 0x8040201008040201;
const BB leftRight = 0x0102040810204080;

const BB whiteSquares = 0xAA55AA55AA55AA55ULL;
const BB blackSquares = 0x55AA55AA55AA55AAULL;

const BB fullBoard = 0xFFFFFFFFFFFFFFFFULL;

const int boardSize = 64;

// defining enums for directions and pieces
enum Color : int {
    WHITE = 0,
    BLACK = 1,
    NONE = 2,
};

// enum for files, don't need one for rank because it's a number already
/*enum Files : int {
    a, b, c, d, e, f, g, h
};*/

enum Board : int {
    // starting with h1 is zero
    h1, g1, f1, e1, d1, c1, b1, a1,
    h2, g2, f2, e2, d2, c2, b2, a2,
    h3, g3, f3, e3, d3, c3, b3, a3,
    h4, g4, f4, e4, d4, c4, b4, a4,
    h5, g5, f5, e5, d5, c5, b5, a5,
    h6, g6, f6, e6, d6, c6, b6, a6,
    h7, g7, f7, e7, d7, c7, b7, a7,
    h8, g8, f8, e8, d8, c8, b8, a8,
};

enum Direction : int {
    NORTH = 8,
    SOUTH = -NORTH,
    EAST = -1,
    WEST = -EAST,
    NORTH_EAST = NORTH + EAST,
    NORTH_WEST = NORTH + WEST,
    SOUTH_EAST = SOUTH + EAST,
    SOUTH_WEST = SOUTH + WEST
};
static const Direction allDirections[] = {NORTH, SOUTH, EAST, WEST, NORTH_EAST, NORTH_WEST, SOUTH_EAST, SOUTH_WEST};
static const Direction squareDirections[] = {NORTH, SOUTH, EAST, WEST};
static const Direction diagonalDirections[] = {NORTH_EAST, NORTH_WEST, SOUTH_WEST, SOUTH_EAST};

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
    Q,
    R,
    B,
    N,
    P,
    k,
    q,
    r,
    b,
    n,
    p
};

// delete later
static void printBitString(BB bitboard) {
    std::cout << "  a b c d e f g h" << std::endl;
    for (int file = 0; file < 8; file ++) {
        std::cout << 8-file;
        std::cout << " ";
        for (int rank = 0; rank < 8; rank ++) {
            std::cout << get_bit(bitboard , (63 - file * 8 - rank));
            std::cout << ' ';
        }
        std::cout << std::endl;
    }
    std::printf("\n");
}


#endif //CHESS_CPP_DEFSENUMS_H

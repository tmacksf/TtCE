//
// Created by Tommy on 1/8/2023.
//

#ifndef CHESS_CPP_PIECE_H
#define CHESS_CPP_PIECE_H
#include "Bitboard.h"

class Piece {
public:
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
        B,
        b,
        N,
        n,
        P,
        p
    };

    std::unordered_map<Pieces, char> pieceChars;
    std::unordered_map<char, Pieces> charsPiece;

    Piece();

    char getChar(int p);
    int getInt(char c);

};

#endif //CHESS_CPP_PIECE_H
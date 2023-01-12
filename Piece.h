//
// Created by Tommy on 1/8/2023.
//

#ifndef CHESS_CPP_PIECE_H
#define CHESS_CPP_PIECE_H
#include <unordered_map>
#include <iostream>

// need to figure out this class

class Piece {
    
public:
    Piece ();

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

    static std::unordered_map<Pieces, char> pieceChars;
    // this can be an array
    static std::unordered_map<char, Pieces> charsPiece;

    static void initMaps();
    static char getChar(int p);
    static int getInt(char c);

};

#endif //CHESS_CPP_PIECE_H
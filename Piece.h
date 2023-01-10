//
// Created by Tommy on 1/8/2023.
//

#ifndef CHESS_CPP_PIECE_H
#define CHESS_CPP_PIECE_H
#include <unordered_map>
#include <iostream>

class Piece {

    // still not super sure if this should be static, but it would make sense
    
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

    std::unordered_map<Pieces, char> pieceChars;
    std::unordered_map<char, Pieces> charsPiece;

    Piece();

    static char getChar(int p);
    static int getInt(char c);

};

#endif //CHESS_CPP_PIECE_H
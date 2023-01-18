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

    static std::unordered_map<int, char> pieceChars;
    // this can be an array
    static std::unordered_map<char, int> charsPiece;

    static void initMaps();
    static char getChar(int p);
    static int getInt(char c);

};

#endif //CHESS_CPP_PIECE_H
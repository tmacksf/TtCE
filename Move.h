//
// Created by Thomas Mack on 05/01/2023.
//

#ifndef CHESS_CPP_MOVE_H
#define CHESS_CPP_MOVE_H
#include "Bitboard.h"

class Move {
public:
    int fromSquare;
    int toSquare;
    int piece;
    bool castleFlag;
    bool enPassantFlag;

    Move(int fromSquare, int toSquare, int piece, bool castleFlag, bool enPassantFlag);
};


#endif //CHESS_CPP_MOVE_H

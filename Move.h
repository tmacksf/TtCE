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
    bool captureFlag;
    bool doublePushFlag;
    bool promotedPiece;
    bool castleFlag;
    bool enPassantFlag;

    Move(int fromSquare, int toSquare, int piece, bool castleFlag, bool enPassantFlag, bool captureFlag, bool doublePushFlag, bool promotedPiece)
        : fromSquare(fromSquare), toSquare(toSquare), piece(piece), castleFlag(castleFlag), enPassantFlag(enPassantFlag),
        captureFlag(captureFlag), doublePushFlag(doublePushFlag), promotedPiece(promotedPiece)
    {}
};


#endif //CHESS_CPP_MOVE_H

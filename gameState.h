//
// Created by Thomas Mack on 28/12/2022.
//

#ifndef CHESS_CPP_GAMESTATE_H
#define CHESS_CPP_GAMESTATE_H

#include "Bitboard.h"

class gameState {
public:
    gameState();
    // order: K, k, Q, q, R, r, N, n, B, b, P, p
    Bitboard bitboards[12];
    int turn;
    // order: white king side, white queen side, black king side, white king side
    bool castling[4];
    int enPassantSquare = -1;

    // member functions
    void printing();
    void initialise(std::string fen);

};


#endif //CHESS_CPP_GAMESTATE_H

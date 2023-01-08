//
// Created by Thomas Mack on 28/12/2022.
//

#ifndef CHESS_CPP_GAMESTATE_H
#define CHESS_CPP_GAMESTATE_H

#include "Piece.h"

class gameState {
public:
    gameState();

    Bitboard bitboards[12]; // order: K, k, Q, q, R, r, B, b, N, n, P, p
    Color turn;
    Color enemy;
    bool castling[4]; // order: white king side, white queen side, black king side, white king side
    int enPassantSquare = -1; // index of the en passant square

    // member functions
    void printing(Piece pcs);
    void initialise(std::string fen);
    BB friendlyBoard();
    BB enemyBoard();
    BB allPieces();
};

#endif //CHESS_CPP_GAMESTATE_H
//
// Created by Thomas Mack on 28/12/2022.
//

#ifndef CHESS_CPP_GAMESTATE_H
#define CHESS_CPP_GAMESTATE_H

#include "Move.h"
#include "Piece.h"


class gameState {
public:
    gameState();

    Bitboard bitboards[12]; // order: K, Q, R, B, N, P, k, q, r, m_bitboard, n, p
    Color turn;
    Color attacking;
    bool castling[4]; // order: white king side, white queen side, black king side, white king side
    int enPassantSquare = -1; // index of the en passant square activated when a pawn is double pushed

    // member functions
    void printing(Piece pcs);
    void initialise(std::string fen);
    BB friendlyBoard();
    BB enemyBoard();
    BB allPieces();
    void moveToGameState(Move &move);
};

#endif //CHESS_CPP_GAMESTATE_H
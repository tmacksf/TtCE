//
// Created by Thomas Mack on 28/12/2022.
//

#ifndef CHESS_CPP_GAMESTATE_H
#define CHESS_CPP_GAMESTATE_H

#include "Move.h"

class gameState {
public:
    gameState();

    Bitboard bitboards[12]; // order: K, Q, R, B, N, P, k, q, r, b, n, p
    int turn; // color based on WHITE = 0, BLACK = 1, NONE = 2
    int attacking;
    bool castling[4]; // order: white king side, white queen side, black king side, white king side
    int enPassantSquare = -1; // index of the en passant square activated when a pawn is double pushed

    // member functions
    void printing();
    void initialise(std::string fen);

    BB inline friendlyBoard() const {
        BB friendly = 0ULL;
        for (int i = 0; i < 6; i++) friendly |= bitboards[i+6*turn].getValue();
        return friendly;
    }

    BB inline enemyBoard() const {
        BB enemy = 0ULL;
        for (int i = 0; i < 6; i++) enemy |= bitboards[i+6*attacking].getValue();
        return enemy;
    }

    BB inline allPieces() const {
        BB all = 0ULL;
        for (Bitboard bitboard : bitboards) all |= bitboard.getValue();
        return all;
    }

    // Move making functions

    void makeMove(Move move);

    void inline removePieceOnCapture(int square) {
        // only have to do half of the list as it is opposition
        BB position = 1ULL << square;
        for (int i = 0 + (6*attacking); i < 6 + (6*attacking); i++) {
            if (position & bitboards[i].getValue()) {
                bitboards[i].unSetBitAt(square);
                break;
            }
        }
    }

    void unMakeMove(Move *moves[]);
    void unMakeMove(Move &move);

    // TODO Clean up and optimize
    BB inline attacked(int position) const {
        BB blockers = allPieces();
        BB friendly = friendlyBoard();

        // leaping pieces
        BB pawns = Bitboard::pawnAttacks[turn][position] & bitboards[PAWN + 6 * attacking].getValue();
        BB knights = Bitboard::knightMoves[position] & bitboards[KNIGHT + 6 * attacking].getValue();
        BB kings = Bitboard::kingMoves[position] & bitboards[KING + 6 * attacking].getValue();

        // sliding pieces
        BB squareAttackers = bitboards[QUEEN + attacking*6].getValue() | bitboards[ROOK + attacking*6].getValue();
        BB square = Magics::getBishopAttacks(position, blockers);
        square = square ^ (square & friendly);

        BB diagonalAttackers = bitboards[BISHOP + attacking*6].getValue() | bitboards[QUEEN + attacking*6].getValue();
        BB diagonals = Magics::getRookAttacks(position, blockers);
        diagonals = diagonals ^ (diagonals & friendly);

        BB all = (diagonals & diagonalAttackers) | (square & squareAttackers) | pawns | knights | kings;

        return all;
    }

    BB inline isKingInCheck() {
        return attacked(Bitboard::getLeastSignificantBit(bitboards[KING + turn * 6].getValue()));
    }
};

#endif //CHESS_CPP_GAMESTATE_H
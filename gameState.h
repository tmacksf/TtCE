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
    Color turn; // color based on WHITE = 0, BLACK = 1, NONE = 2
    Color attacking;
    bool castling[4]; // order: white king side, white queen side, black king side, white king side
    int enPassantSquare = -1; // index of the en passant square activated when a pawn is double pushed

    // optional helper variables

    // this can be used in move generation to generate evasions and blocks for that attacker
    int attackerLocation = -1;

    // member functions
    void printing();
    void stateToFen();
    void initialise(std::string fen);

    int getPieceAt(int square) {
        int piece = 0;
        for (Bitboard bitboard : bitboards) {
            if (bitboard.getValue() & 1ULL << square) return piece;
            piece += 1;
        }
        throw std::runtime_error("No piece at position");
    }

    [[nodiscard]] BB inline friendlyBoard() const {
        BB friendly = 0ULL;
        for (int i = 0; i < 6; i++) friendly |= bitboards[i+6*turn].getValue();
        return friendly;
    }

    [[nodiscard]] BB inline enemyBoard() const {
        BB enemy = 0ULL;
        for (int i = 0; i < 6; i++) enemy |= bitboards[i+6*attacking].getValue();
        return enemy;
    }

    [[nodiscard]] BB inline allPieces() const {
        BB all = 0ULL;
        for (Bitboard bitboard : bitboards) all |= bitboard.getValue();
        return all;
    }

    [[nodiscard]] BB inline getPieceBitboard(PieceTypes pt, Color c) const {
        return bitboards[pt + 6*c].getValue();
    }

    static int inline getPiece(PieceTypes pt, Color c) {
        return pt + 6*c;
    }

    // Move making functions

    void makeMove(const Move &move);

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
    // Method to see if a square is attacked, take position and attacking color
    [[nodiscard]] int inline attacked(int position, Color attackingColor) const {
        BB allBlockers = allPieces();
        Color friendlyColor = ~attackingColor;

        // TODO Fix this function
        int attackers = 0;
        // leaping pieces
        //attackers += Bitboard::bitCount(Bitboard::pawnAttacks[friendlyColor][position] & bitboards[getPiece(PAWN, attackingColor)].getValue());
        if (Bitboard::pawnAttacks[friendlyColor][position] & bitboards[PAWN + 6 * attackingColor].getValue()) attackers += 1;
        if (Bitboard::knightMoves[position] & bitboards[KNIGHT + 6 * attackingColor].getValue()) attackers += 1;
        if (Bitboard::kingMoves[position] & bitboards[KING + 6 * attackingColor].getValue()) attackers += 1;

        // sliding pieces
        BB squareAttackers = bitboards[QUEEN + attackingColor*6].getValue() | bitboards[ROOK + attackingColor*6].getValue();
        BB square = Magics::getRookAttacks(position, allBlockers);
        if (square & squareAttackers) attackers += 1;

        BB diagonalAttackers = bitboards[BISHOP + attackingColor*6].getValue() | bitboards[QUEEN + attackingColor*6].getValue();
        BB diagonals = Magics::getBishopAttacks(position, allBlockers);
        if (diagonals & diagonalAttackers) attackers += 1;

        return attackers;
    }

    [[nodiscard]] BB inline isKingInCheck(Color attackingColor) const {
        return attacked(Bitboard::getLeastSignificantBit(bitboards[KING + (1-attackingColor) * 6].getValue()), attackingColor);
    }
};

#endif //CHESS_CPP_GAMESTATE_H
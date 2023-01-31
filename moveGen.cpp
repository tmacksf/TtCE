//
// Created by Thomas Mack on 23/12/2022.
//

#include "moveGen.h"

void moveGen::pseudoLegalMoves(gameState gs, std::vector<Move> &moves) {
    int turn = gs.turn;
    BB allPieces = gs.allPieces();
    BB friendlyPieces = gs.friendlyBoard();
    BB enemyPieces = gs.enemyBoard();

    // TODO optimize passing values and references so we are passing pointers and not copies of values (in some cases passing by value is necessary)
    // we start with leaping piece moves. Pawn, knight, king
    // pawn attacks and moves
    BB pawns = gs.bitboards[PAWN + 6 * turn].getValue();
    int enPassantSquare = gs.enPassantSquare;
    pawnMoves(turn, pawns, friendlyPieces, moves);
    pawnAttacks(turn, enPassantSquare, pawns, allPieces, friendlyPieces, moves);

    // knight moves
    BB knights = gs.bitboards[KNIGHT + 6 * turn].getValue();
    knightMoves(turn, knights, friendlyPieces, enemyPieces, moves);

    // king moves
    BB king = gs.bitboards[KING + 6 * turn].getValue();
    kingMoves(turn, king, friendlyPieces, enemyPieces, moves);

    // TODO sliding pieces
    // Sliding pieces

    /*rookMoves();

    bishopMoves();

    queenMoves();*/

}

void moveGen::legalMoves(gameState gs, std::vector<Move> &moves) {
    // first will check if king is in check then will act accordingly

    // will do castling here

}

// need to add checking for en passant moves in here too as there is a flag in the game-state, so we can just get the coords from that
void moveGen::pawnMoves(int turn, BB pawns, BB allPieces, std::vector<Move> &moves) {
    int pawnIndex;
    while (pawns) {
        pawnIndex = Bitboard::getLeastSignificantBit(pawns);

        // getting the move table for the pawn at the position
        BB moves_bb = Bitboard::pawnMoves[turn][pawnIndex];
        int moveIndex;
        bool doublePush = false;
        while (moves_bb) {
            moveIndex = Bitboard::getLeastSignificantBit(moves_bb);

            // no need to check for double push if there is a piece in front
            if (allPieces & (1ULL << moveIndex)) {
                break;
            }

            Move move{pawnIndex, moveIndex, PAWN + 6*turn, false, false, false, doublePush, false};
            moves.push_back(move);
            doublePush = true;
            unset_bit(moves_bb, moveIndex);
        }
        // unsetting the bit to do the next pawn
        // either change everything like this to macros or change it all to static inline bitboard functions
        unset_bit(pawns, pawnIndex);
    }
}

void moveGen::pawnAttacks(int turn, int enPassantSquare, BB pawns, BB allPieces, BB friendlyPieces, std::vector<Move> &moves) {
    int pawnIndex;
    while (pawns) {
        pawnIndex = Bitboard::getLeastSignificantBit(pawns);

        // getting the attack table for the pawn at the position
        BB pawnAttacks = Bitboard::pawnAttacks[turn][pawnIndex];

        // going to do the same thing where we loop through the bitboard until there are no more pawnAttacks remaining.
        // Each time a move will be added
        int pawnAttackIndex = 0;
        while (pawnAttacks) {
            pawnAttackIndex = Bitboard::getLeastSignificantBit(pawnAttacks);

            // en passant
            if (pawnAttackIndex == enPassantSquare) {
                Move move{pawnIndex, pawnAttackIndex, PAWN + 6 * turn, false, true, true, false, false};
                unset_bit(pawnAttacks, pawnAttackIndex);
                moves.push_back(move);
                continue;
            }

            // we check if the position in a valid attack, meaning is there an enemy piece (includes en passant)
            if ((friendlyPieces & (1ULL << pawnAttackIndex)) or !(allPieces & (1ULL << pawnAttackIndex))) {
                // invalid attack means a move will not be added, so we unset the bit and go to the next iteration
                unset_bit(pawnAttacks, pawnAttackIndex);
                continue;
            }
            // need to figure out how to make a move here that will remain in the vector after this goes out of scope
            Move move{pawnIndex, pawnAttackIndex, PAWN + 6 * turn, false, false, true, false, false};
            moves.push_back(move);
            // pawnAttacks = 0ULL;
            unset_bit(pawnAttacks, pawnAttackIndex);
        }
        unset_bit(pawns, pawnIndex);
    }
}

void moveGen::knightMoves(int turn, BB knights, BB friendlyPieces, BB enemyPieces, std::vector<Move> &moves) {
    int knightIndex;
    while (knights) {
        knightIndex = Bitboard::getLeastSignificantBit(knights);

        // getting the attack table for the pawn at the position
        BB knightMoves = Bitboard::knightMoves[knightIndex];

        int knightAttackIndex = 0;
        while (knightMoves) {
            knightAttackIndex = Bitboard::getLeastSignificantBit(knightMoves);

            if (friendlyPieces & (1ULL << knightAttackIndex)) {
                unset_bit(knightMoves, knightAttackIndex);
                continue;
            }
            bool captureFlag = false;
            // TODO captureFlag = enemyPieces & (1ULL << knightAttackIndex) because this will return non-zero if it's a capture resulting in less branching
            if (enemyPieces & (1ULL << knightAttackIndex)){
                captureFlag = true;
            }

            Move move{knightIndex, knightAttackIndex, KNIGHT + 6 * turn, false, false, captureFlag, false, false};
            moves.push_back(move);
            unset_bit(knightMoves, knightAttackIndex);
        }
        unset_bit(knights, knightIndex);
    }
}

void moveGen::kingMoves(int turn, BB king, BB friendlyPieces, BB enemyPieces, std::vector<Move> &moves) {
    // TODO Castling
    int kingIndex = Bitboard::getLeastSignificantBit(king);
    BB kingMoves = Bitboard::kingMoves[kingIndex];
    int kingMoveIndex = 0;
    while (kingMoves) {
        kingMoveIndex = Bitboard::getLeastSignificantBit(kingMoves);

        if (friendlyPieces & (1ULL << kingMoveIndex)) {
            unset_bit(kingMoves, kingMoveIndex);
            continue;
        }
        bool captureFlag = false;
        // TODO same here as the knight moves where a capture would result in a number and a non capture would lead to 0
        if (enemyPieces & (1ULL << kingMoveIndex)) {
            captureFlag = true;
        }
        Move move{kingIndex, kingMoveIndex, KING + 6 * turn, false, false, captureFlag, false, false};
        moves.push_back(move);
        unset_bit(kingMoves, kingMoveIndex);
    }
}

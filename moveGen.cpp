//
// Created by Thomas Mack on 23/12/2022.
//

#include "moveGen.h"

void moveGen::pseudoLegalMoves(const gameState &gs, std::vector<Move> &moves) {
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
    pawnAttacks(turn, enPassantSquare, pawns, enemyPieces, moves);

    // knight moves
    BB knights = gs.bitboards[KNIGHT + 6 * turn].getValue();
    knightMoves(turn, knights, friendlyPieces, enemyPieces, moves);

    // king moves
    kingMoves(gs, moves);

    // Sliding pieces
    BB rooks = gs.bitboards[ROOK + 6 * turn].getValue();

    rookMoves(turn, rooks, allPieces, friendlyPieces, enemyPieces, moves);

    BB bishops = gs.bitboards[BISHOP + 6 * turn].getValue();
    bishopMoves(turn, bishops, allPieces, friendlyPieces, enemyPieces, moves);

    BB queens = gs.bitboards[QUEEN + 6 * turn].getValue();
    queenMoves(turn, queens, allPieces, friendlyPieces, enemyPieces, moves);
}

// TODO optimize a lot (There is a ton to do here but this is an easy start)
void moveGen::legalMoves(const gameState &gs, std::vector<Move> &legalMoves) {
    std::vector<Move> pseudoLegalMoveVector;
    pseudoLegalMoveVector.reserve(16);
    pseudoLegalMoves(gs, pseudoLegalMoveVector);
    legalMoves.reserve(sizeof(pseudoLegalMoveVector));
    for (Move move : pseudoLegalMoveVector) {
        gameState gsCopy = gs;
        gsCopy.makeMove(move);
        if (!gsCopy.isKingInCheck()) {
            legalMoves.push_back(move);
        }
    }
}

// need to add checking for en passant moves in here too as there is a flag in the game-state, so we can just get the coords from that
void moveGen::pawnMoves(int turn, BB pawns, BB allPieces, std::vector<Move> &moves) {
    int pawnIndex;
    while (pawns) {
        pawnIndex = Bitboard::getLeastSignificantBit(pawns);

        // getting the move table for the pawn at the position
        // BB moves_bb = Bitboard::pawnMoves[turn][pawnIndex] ^ (Bitboard::pawnMoves[turn][pawnIndex] & allPieces);
        BB moves_bb = Bitboard::pawnMoves[turn][pawnIndex];
        int moveIndex;
        bool doublePush = false;
        while (moves_bb) {
            moveIndex = Bitboard::getLeastSignificantBit(moves_bb);

            // no need to check for double push if there is a piece in front
            if (allPieces & (1ULL << moveIndex)) {
                break;
            }

            moves.emplace_back(pawnIndex, moveIndex, PAWN + 6*turn, false, false, false, doublePush, false);
            doublePush = true;
            unset_bit(moves_bb, moveIndex);
        }
        // unsetting the bit to do the next pawn
        // either change everything like this to macros or change it all to static inline bitboard functions
        unset_bit(pawns, pawnIndex);
    }
}

void moveGen::pawnAttacks(int turn, int enPassantSquare, BB pawns, BB enemyPieces, std::vector<Move> &moves) {
    int pawnIndex;
    // getting bitboard of the square which the pawn can move to after an opposing pawn has double pushed allowing for en passant
    BB enPassantBitboard = 0ULL;
    bool enPassantFlag = false;
    if (enPassantSquare != -1) enPassantBitboard = 1ULL << enPassantSquare;
    while (pawns) {
        pawnIndex = Bitboard::getLeastSignificantBit(pawns);

        // getting the attack table for the pawn at the position
        BB pawnAttacks = Bitboard::pawnAttacks[turn][pawnIndex] & enemyPieces;
        pawnAttacks |= Bitboard::pawnAttacks[turn][pawnIndex] & enPassantBitboard;

        int pawnAttackIndex = 0;
        while (pawnAttacks) {
            pawnAttackIndex = Bitboard::getLeastSignificantBit(pawnAttacks);

            if (pawnAttackIndex == enPassantSquare) enPassantFlag = true;

            moves.emplace_back(pawnIndex, pawnAttackIndex, PAWN + 6 * turn, false, enPassantFlag, true, false, false);
            // don't want to set this every time
            enPassantFlag = false;
            unset_bit(pawnAttacks, pawnAttackIndex);
        }
        unset_bit(pawns, pawnIndex);
    }
}

void moveGen::knightMoves(int turn, BB knights, BB friendlyPieces, BB enemyPieces, std::vector<Move> &moves) {
    int knightIndex;
    while (knights) {
        knightIndex = Bitboard::getLeastSignificantBit(knights);
        BB knightMoves = Bitboard::knightMoves[knightIndex] ^ (Bitboard::knightMoves[knightIndex] & friendlyPieces);

        int knightAttackIndex = 0;
        while (knightMoves) {
            knightAttackIndex = Bitboard::getLeastSignificantBit(knightMoves);
            bool captureFlag = enemyPieces & (1ULL << knightAttackIndex);

            moves.emplace_back(knightIndex, knightAttackIndex, KNIGHT + 6 * turn, false, false, captureFlag, false, false);
            unset_bit(knightMoves, knightAttackIndex);
        }
        unset_bit(knights, knightIndex);
    }
}

void moveGen::kingMoves(const gameState &gs, std::vector<Move> &moves) {
    int turn = gs.turn;
    BB king = gs.bitboards[KING + turn * 6].getValue();
    BB friendlyPieces = gs.friendlyBoard();
    BB enemyPieces = gs.enemyBoard();
    BB allPieces = friendlyPieces | enemyPieces;
    // TODO Castling
    int kingIndex = Bitboard::getLeastSignificantBit(king);
    BB kingAttacks = Bitboard::kingMoves[kingIndex] ^ (Bitboard::kingMoves[kingIndex] & friendlyPieces);

    while (kingAttacks) {
        int kingMoveIndex = Bitboard::getLeastSignificantBit(kingAttacks);
        bool captureFlag = enemyPieces & (1ULL << kingMoveIndex);
        moves.emplace_back(kingIndex, kingMoveIndex, KING + (6 * turn), false, false, captureFlag, false, false);
        unset_bit(kingAttacks, kingMoveIndex);
    }
    // castling
    // if it is black's turn
    if (turn) {
        if (gs.castling[2] and !(allPieces & (3ULL << 57))) {
            if (!(gs.attacked(g8) | gs.attacked(f8))) {
                moves.emplace_back(kingIndex, g8, KING+6, true, false, false, false, false);
            }
        }
        if (gs.castling[3] and !(allPieces & (7ULL << 60))) {
            if (!(gs.attacked(d8) | gs.attacked(c8))) {
                moves.emplace_back(kingIndex, c8, KING+6, true, false, false, false, false);
            }
        }
    }
    // if it is white
    else {
        // king side
        // checking to see if we can still castle and if there are no pieces in the way of castling
        if (gs.castling[0] and !(allPieces & (3ULL << 1))) {
            if (!(gs.attacked(g1) | gs.attacked(f1))) {
                moves.emplace_back(kingIndex, g1, WHITE, true, false, false, false, false);
            }
        }
        // queen side
        if (gs.castling[1] and !(allPieces & (7ULL << 4))) {
            if (!(gs.attacked(d1) | gs.attacked(c1))) {
                moves.emplace_back(kingIndex, c1, WHITE, true, false, false, false, false);
            }
        }
    }
}

void moveGen::rookMoves(int turn, BB rooks, BB allPieces, BB friendlyPieces, BB enemyPieces, std::vector<Move> &moves) {
    int rookIndex;
    while (rooks) {
        // cycle through rooks
        rookIndex = Bitboard::getLeastSignificantBit(rooks);

        BB rookMoves = Magics::getRookAttacks(rookIndex, allPieces);

        // removing overlap with friendly pieces
        rookMoves ^= rookMoves & friendlyPieces;
        BB moveIndex;
        while (rookMoves) {
            moveIndex = Bitboard::getLeastSignificantBit(rookMoves);

            bool captureFlag = (1ULL << moveIndex) & enemyPieces;
            // using emplace back to avoid copies
            moves.emplace_back(rookIndex, moveIndex, ROOK + 6*turn, false, false, captureFlag, false, false);
            unset_bit(rookMoves, moveIndex);
        }
        unset_bit(rooks, rookIndex);
    }
}

void moveGen::bishopMoves(int turn, BB bishops, BB allPieces, BB friendlyPieces, BB enemyPieces, std::vector<Move> &moves) {
    int bishopIndex;
    while (bishops) {
        bishopIndex = Bitboard::getLeastSignificantBit(bishops);
        BB bishopMoves = Magics::getBishopAttacks(bishopIndex, allPieces);

        bishopMoves ^= bishopMoves & friendlyPieces;
        BB moveIndex;
        while (bishopMoves) {
            moveIndex = Bitboard::getLeastSignificantBit(bishopMoves);
            bool captureFlag = (1ULL << moveIndex) & enemyPieces;

            moves.emplace_back(bishopIndex, moveIndex, BISHOP + 6*turn, false, false, captureFlag, false, false);
            unset_bit(bishopMoves, moveIndex);
        }
        unset_bit(bishops, bishopIndex);
    }
}

void moveGen::queenMoves(int turn, BB queens, BB allPieces, BB friendlyPieces, BB enemyPieces, std::vector<Move> &moves) {
    int queenIndex;
    while (queens) {
        queenIndex = Bitboard::getLeastSignificantBit(queens);
        BB queenMoves = Magics::getBishopAttacks(queenIndex, allPieces) | Magics::getRookAttacks(queenIndex, allPieces);
        queenMoves ^= queenMoves & friendlyPieces;
        BB moveIndex;
        while (queenMoves) {
            moveIndex = Bitboard::getLeastSignificantBit(queenMoves);
            bool captureFlag = (1ULL << moveIndex) & enemyPieces;
            moves.emplace_back(queenIndex, moveIndex, QUEEN + 6*turn, false, false, captureFlag, false, false);
            unset_bit(queenMoves, moveIndex);
        }
        unset_bit(queens, queenIndex);
    }
}

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

    BB rooks = gs.bitboards[ROOK + 6 * turn].getValue();
    rookMoves(turn, rooks, allPieces, friendlyPieces, enemyPieces, moves);

    BB bishops = gs.bitboards[BISHOP + 6* turn].getValue();
    bishopMoves(turn, bishops, allPieces, friendlyPieces, enemyPieces, moves);

    BB queens = gs.bitboards[QUEEN + 6*turn].getValue();
    queenMoves(turn, queens, allPieces, friendlyPieces, enemyPieces, moves);
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
            bool captureFlag = enemyPieces & (1ULL << knightAttackIndex);

            moves.emplace_back(knightIndex, knightAttackIndex, KNIGHT + 6 * turn, false, false, captureFlag, false, false);
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
        bool captureFlag = enemyPieces & (1ULL << kingMoveIndex);

        moves.emplace_back(kingIndex, kingMoveIndex, KING + 6 * turn, false, false, captureFlag, false, false);
        unset_bit(kingMoves, kingMoveIndex);
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

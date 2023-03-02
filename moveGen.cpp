//
// Created by Thomas Mack on 23/12/2022.
//

#include "moveGen.h"

// TODO optimize a lot (There is a ton to do here but this is an easy start)
void moveGen::legalMoves(const gameState &gs, std::vector<Move> &legalMoves) {
    std::vector<Move> pseudoLegalMoveVector;
    pseudoLegalMoveVector.reserve(16);
    legalMoves.reserve(pseudoLegalMoveVector.size());

    /*int checkNum = gs.isKingInCheck(gs.attacking);
    // if king is in check
    if (checkNum) {
        if (checkNum > 1) {
            kingMoves(gs, pseudoLegalMoveVector);
            for (Move m : pseudoLegalMoveVector) {
                gameState gsCopy = gs;
                gsCopy.makeMove(m);
                if (!gsCopy.isKingInCheck(gs.attacking)) legalMoves.push_back(m);
            }
        }
        else {
            // need to generate evasions here
        }
    }
    else {
        // get location of king
        int kingSquare = gs.getPieceBitboard(KING, gs.turn);

        // ignores friendly pieces to look for attacks on the king
        BB maybePinned = Magics::getQueenAttacks(kingSquare, gs.enemyBoard());
        // rough way of doing checks could be to see if 1ULL << fromSquare is on the king line. Only then will the move be made and there will be a check to see if the king is in check

        pseudoLegalMoves(gs, pseudoLegalMoveVector);

        for (Move move : pseudoLegalMoveVector) {
            gameState gsCopy = gs;
            gsCopy.makeMove(move);
            if (!gsCopy.isKingInCheck(gs.attacking)) {
                legalMoves.push_back(move);
            }
        }
    }*/

    pseudoLegalMoves(gs, pseudoLegalMoveVector);

    for (Move move : pseudoLegalMoveVector) {
        gameState gsCopy = gs;
        gsCopy.makeMove(move);
        if (!gsCopy.isKingInCheck(gs.getAttacking())) {
            legalMoves.push_back(move);
        }
    }
}

void moveGen::pseudoLegalMoves(const gameState &gs, std::vector<Move> &moves) {
    Color turn = gs.getTurn();

    // TODO optimize passing values and references so we are passing pointers and not copies of values (in some cases passing by value is necessary)
    // we start with leaping piece moves. Pawn, knight, king
    // pawn attacks and moves
    if (turn) {
        moveGen::pawnMoves<BLACK, Quiet>(gs, moves);
        moveGen::pawnMoves<BLACK, Captures>(gs, moves);
        moveGen::pieceMoves<BLACK, QUEEN, All>(gs, moves);
        moveGen::pieceMoves<BLACK, ROOK, All>(gs, moves);
        moveGen::pieceMoves<BLACK, BISHOP, All>(gs, moves);
        moveGen::pieceMoves<BLACK, KNIGHT, All>(gs, moves);
    }
    else {
        moveGen::pawnMoves<WHITE, Quiet>(gs, moves);
        moveGen::pawnMoves<WHITE, Captures>(gs, moves);
        moveGen::pieceMoves<WHITE, QUEEN, All>(gs, moves);
        moveGen::pieceMoves<WHITE, ROOK, All>(gs, moves);
        moveGen::pieceMoves<WHITE, BISHOP, All>(gs, moves);
        moveGen::pieceMoves<WHITE, KNIGHT, All>(gs, moves);
    }

    kingMoves(gs, moves);
}

void moveGen::kingMoves(const gameState &gs, std::vector<Move> &moves) {
    Color turn = gs.getTurn();
    Color attacking = ~turn;
    BB king = gs.getPieceBitboard(KING, turn);
    BB friendlyPieces = gs.friendlyBoard();
    BB enemyPieces = gs.enemyBoard();
    int kingIndex = Bitboard::getLeastSignificantBit(king);
    BB kingAttacks = Bitboard::kingMoves[kingIndex] ^ (Bitboard::kingMoves[kingIndex] & friendlyPieces);

    bool captureFlag = false;
    while (kingAttacks) {
        int kingMoveIndex = pop_lsb(kingAttacks);
        if ((1ULL << kingMoveIndex) & enemyPieces) captureFlag = true;
        //bool captureFlag = enemyPieces & (1ULL << kingMoveIndex);
        moves.emplace_back(kingIndex, kingMoveIndex, KING + (6 * turn), false, false, captureFlag, false, false);
        captureFlag = false;
    }
    // castling
    // if it is black's turn
    // todo make sure that the king cannot castle while in check
    if (!gs.isKingInCheck(attacking)) {
        if (turn) kingCastling<BLACK>(kingIndex, gs, moves);
        else kingCastling<WHITE>(kingIndex, gs, moves);
    }
}

/*
void moveGen::pawnMoves(int turn, BB pawns, BB allPieces, std::vector<Move> &moves) {
    // setting up a bunch of variables which will help with the speed of
    BB promotionRank = 0xFF00000000000000ULL;

    if (turn) {
        promotionRank = 0xFFULL;
    }

    int pawnIndex;
    while (pawns) {
        pawnIndex = Bitboard::getLeastSignificantBit(pawns);
        BB moves_bb = Bitboard::pawnMoves[turn][pawnIndex];
        int moveIndex;
        bool doublePush = false;
        while (moves_bb) {
            moveIndex = Bitboard::getLeastSignificantBit(moves_bb);

            // no need to check for double push if there is a piece in front
            if (allPieces & (1ULL << moveIndex)) {
                break;
            }

            // handling pawn promotions (Need to do this a better way in the future)
            // added unlikely because pawn promotions are kinda rare
            if (moveIndex & promotionRank) [[unlikely]]{
                // adding 4 moves for the different types of promotions
                moves.emplace_back(pawnIndex, moveIndex, PAWN + 6*turn, false, false, false, false, QUEEN + 6*turn);
                moves.emplace_back(pawnIndex, moveIndex, PAWN + 6*turn, false, false, false, false, ROOK + 6*turn);
                moves.emplace_back(pawnIndex, moveIndex, PAWN + 6*turn, false, false, false, false, BISHOP + 6*turn);
                moves.emplace_back(pawnIndex, moveIndex, PAWN + 6*turn, false, false, false, false, KNIGHT + 6*turn);
                unset_bit(moves_bb, moveIndex);
                continue;
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
    BB promotionRank = rank7;
    if (turn) {
        promotionRank = rank2;
    }
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

            if (pawnAttackIndex & promotionRank) [[unlikely]]{
                // adding 4 moves for the different types of promotions
                moves.emplace_back(pawnIndex, pawnAttackIndex, PAWN + 6*turn, false, false, false, false, QUEEN + 6*turn);
                moves.emplace_back(pawnIndex, pawnAttackIndex, PAWN + 6*turn, false, false, false, false, ROOK + 6*turn);
                moves.emplace_back(pawnIndex, pawnAttackIndex, PAWN + 6*turn, false, false, false, false, BISHOP + 6*turn);
                moves.emplace_back(pawnIndex, pawnAttackIndex, PAWN + 6*turn, false, false, false, false, KNIGHT + 6*turn);
                unset_bit(pawnAttacks, pawnAttackIndex);
                continue;
            }

            moves.emplace_back(pawnIndex, pawnAttackIndex, PAWN + (6 * turn), false, enPassantFlag, true, false, false);
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
            // knightAttackIndex = pop_lsb(knightMoves);
            bool captureFlag = enemyPieces & (1ULL << knightAttackIndex);

            moves.emplace_back(knightIndex, knightAttackIndex, KNIGHT + 6 * turn, false, false, captureFlag, false, false);
            unset_bit(knightMoves, knightAttackIndex);
        }
        unset_bit(knights, knightIndex);
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
}*/
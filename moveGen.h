//
// Created by Thomas Mack on 23/12/2022.
//

#ifndef CHESS_CPP_MOVEGEN_H
#define CHESS_CPP_MOVEGEN_H
#include "gameState.h"

class moveGen {
public:
    moveGen()= default;
    static void legalMoves(const gameState &gs, std::vector<Move> &moves);
    static void pseudoLegalMoves(const gameState &gs, std::vector<Move> &moves);

    // leaping piece moves
    //static void pawnMoves(int turn, BB pawns, BB allPieces, std::vector<Move> &moves);
    //static void pawnAttacks(int turn, int enPassantSquare, BB pawns, BB enemyPieces, std::vector<Move> &moves);
    //static void knightMoves(int turn, BB knights, BB friendlyPieces, BB enemyPieces, std::vector<Move> &moves);
    static void kingMoves(const gameState &gs, std::vector<Move> &moves);

    // Adds the squares the king can castle on to the
    template<Color turn>
    static void inline kingCastling(const int kingIndex, const gameState &gs, std::vector<Move> &moves) {
        constexpr int kingSideIndex = (turn == BLACK ? 2 : 0);
        constexpr int queenSideIndex = (turn == BLACK ? 3 : 1);
        // todo make sure this is right
        constexpr BB kingSideOccupancy = (turn == BLACK ? (3ULL << 57) : (3ULL << 1));
        constexpr BB queenSideOccupancy = (turn == BLACK ? (7ULL << 60) : (7ULL << 4));
        // not sure how to better do this but for now this is okay. Square 1 is the one that the king moves to on a castle
        constexpr int kingSideSquareOne = (turn == BLACK ? g8 : g1);
        constexpr int kingSideSquareTwo = (turn == BLACK ? f8 : f1);
        constexpr int queenSideSquareOne = (turn == BLACK ? c8 : c1);
        constexpr int queenSideSquareTwo = (turn == BLACK ? d8 : d1);
        int piece = gameState::getPiece(KING, turn);

        // TODO make sure that changing the availability of castling within this method is okay
        // checking to see if the rook is in the right position
        // these get the bitboard of the space where the rook should be on each side
        constexpr BB rookKingSide = (turn == BLACK ? 1ULL << 56 : 1ULL);
        constexpr BB rookQueenSide = (turn == BLACK ? 1ULL << 63 : 1ULL << 7);
        int rook = gameState::getPiece(ROOK, turn);
        BB rookBitboard = gs.getPieceBitboard(ROOK, turn);

        const BB allPieces = gs.allPieces();

        // king side castling
        if (gs.getCastling(kingSideIndex) and !(allPieces & (kingSideOccupancy))) {
            // are the squares attacked
            if (rookKingSide & rookBitboard) {
                if (!gs.attacked(kingSideSquareOne, gs.getAttacking()) and !gs.attacked(kingSideSquareTwo, gs.getAttacking())) {
                moves.emplace_back(kingIndex, kingSideSquareOne, piece, true, false, false, false, false);
                }
            }
        }
        // queen side castling
        if (gs.getCastling(queenSideIndex) and !(allPieces & queenSideOccupancy)) {
            // are the squares attacked
            if (rookQueenSide & rookBitboard) {
                if (!gs.attacked(queenSideSquareOne, gs.getAttacking()) and !gs.attacked(queenSideSquareTwo, gs.getAttacking())) {
                    moves.emplace_back(kingIndex, queenSideSquareOne, piece, true, false, false, false, false);
                }
            }
        }
    }

    // sliding attacks
    //static void rookMoves(int turn, BB rooks, BB allPieces, BB friendlyPieces, BB enemyPieces, std::vector<Move> &moves);
    //static void bishopMoves(int turn, BB bishops, BB allPieces, BB friendlyPieces, BB enemyPieces, std::vector<Move> &moves);
    //static void queenMoves(int turn, BB queens, BB allPieces, BB friendlyPieces, BB enemyPieces, std::vector<Move> &moves);

    // Section for templates
    template<Color turn, MoveType Type>
    static void pawnMoves(const gameState &gs, std::vector<Move> &moves) {
        constexpr BB promotionRank = (turn == BLACK ? rank2 : rank7);
        constexpr BB firstPushRank = (turn == BLACK ? rank6 : rank3);
        constexpr Direction direction = (turn == BLACK ? SOUTH : NORTH);
        constexpr Direction upRight  = (turn == WHITE ? NORTH_EAST : SOUTH_WEST);
        constexpr Direction upLeft   = (turn == WHITE ? NORTH_WEST : SOUTH_EAST);
        constexpr BB enPassantRank = (turn == BLACK ? rank3 : rank6);
        int piece = gameState::getPiece(PAWN, turn);

        const BB emptySquares = ~gs.allPieces();
        const BB pawns = gs.getPieceBitboard(PAWN, turn);
        const BB enemyBoard = gs.enemyBoard();

        // setting up for pawn promotions
        BB pawnPromotions = promotionRank & pawns;
        BB otherPawns = pawns & ~promotionRank;

        if (pawnPromotions && Type == Quiet) {
            BB promotions = Bitboard::shift<direction>(pawnPromotions) & emptySquares;
            while (promotions) {
                // handling promotions
                int to = pop_lsb(promotions);
                moves.emplace_back(to - direction, to, piece, false, false, false, false, QUEEN + 6*turn);
                moves.emplace_back(to - direction, to, piece, false, false, false, false, ROOK + 6*turn);
                moves.emplace_back(to - direction, to, piece, false, false, false, false, BISHOP + 6*turn);
                moves.emplace_back(to - direction, to, piece, false, false, false, false, KNIGHT + 6*turn);
            }

        }
        if (pawnPromotions && Type == Captures) {
            BB rightCapture = Bitboard::shift<upRight>(pawnPromotions) & enemyBoard;
            BB leftCapture = Bitboard::shift<upLeft>(pawnPromotions) & enemyBoard;
            while (rightCapture) {
                int to = pop_lsb(rightCapture);
                moves.emplace_back(to - upRight, to, piece, false, false, true, false, QUEEN + 6*turn);
                moves.emplace_back(to - upRight, to, piece, false, false, true, false, ROOK + 6*turn);
                moves.emplace_back(to - upRight, to, piece, false, false, true, false, BISHOP + 6*turn);
                moves.emplace_back(to - upRight, to, piece, false, false, true, false, KNIGHT + 6*turn);
            }
            while (leftCapture) {
                int to = pop_lsb(leftCapture);
                moves.emplace_back(to - upLeft, to, piece, false, false, true, false, QUEEN + 6*turn);
                moves.emplace_back(to - upLeft, to, piece, false, false, true, false, ROOK + 6*turn);
                moves.emplace_back(to - upLeft, to, piece, false, false, true, false, BISHOP + 6*turn);
                moves.emplace_back(to - upLeft, to, piece, false, false, true, false, KNIGHT + 6*turn);
            }
        }

        if constexpr (Type == Captures) {
            BB rightCapture = Bitboard::shift<upRight>(otherPawns) & enemyBoard;
            BB leftCapture = Bitboard::shift<upLeft>(otherPawns) & enemyBoard;

            while (rightCapture) {
                int to = pop_lsb(rightCapture);
                moves.emplace_back(to - upRight, to, piece, false, false, true, false, false);
            }
            while (leftCapture) {
                int to = pop_lsb(leftCapture);
                moves.emplace_back(to - upLeft, to, piece, false, false, true, false, false);
            }

            // TODO handle en passant
            if (gs.getEPSquare() != -1) {
                // TODO add assert that its on the (relative) 6th rank
                assert(gs.getEPSquare() < 64 and gs.getEPSquare() >= 0);
                BB epPawns = Bitboard::pawnAttacks[~turn][gs.getEPSquare()] & pawns;
                while (epPawns) {
                    moves.emplace_back(pop_lsb(epPawns), gs.getEPSquare(), piece, false, true, true, false, false);
                }
            }
        }

        if constexpr (Type == Quiet) {
            BB firstPush = Bitboard::shift<direction>(otherPawns) & emptySquares;
            BB secondPush = Bitboard::shift<direction>(firstPush & firstPushRank) & emptySquares;

            while (firstPush) {
                int to = pop_lsb(firstPush);
                moves.emplace_back(to - direction, to, piece, false, false, false, false, false);
            }

            while (secondPush) {
                int to = pop_lsb(secondPush);
                moves.emplace_back(to - direction*2, to, piece, false, false, false, true, false);
            }
        }
    }

    template<Color turn, PieceTypes pt, MoveType Type>
    static void pieceMoves(const gameState &gs, std::vector<Move> &moves) {
        BB pieceBitboard = gs.getPieceBitboard(pt, turn);
        BB enemyPieces = gs.enemyBoard();
        BB friendlyPieces = gs.friendlyBoard();
        BB allPieces = gs.friendlyBoard() | gs.enemyBoard(); // slightly faster than using gs.allPieces();
        int piece = gameState::getPiece(pt, turn);

        if constexpr (Type == Captures) {
            // TODO put the from/to inside the emplace back to reduce time
            while (pieceBitboard) {
                int from = pop_lsb(pieceBitboard);
                BB attacks = attacksBitboard<pt>(from, allPieces) & enemyPieces;
                while (attacks) {
                    int to = pop_lsb(attacks);
                    moves.emplace_back(from, to, piece, false, false, true, false, false);
                }
            }
        }

        if constexpr (Type == Quiet) {
            while (pieceBitboard) {
                int from = pop_lsb(pieceBitboard);
                // todo make sure that this works for generating possible moves
                BB attacks = attacksBitboard<pt>(from, allPieces) & ~allPieces;
                while (attacks) {
                    int to = pop_lsb(attacks);
                    moves.emplace_back(from, to, piece, false, false, false, false, false);
                }
            }
        }

        if constexpr (Type == All) {
            bool capture = false;
            while (pieceBitboard) {
                const int from = pop_lsb(pieceBitboard);

                BB attacks = attacksBitboard<pt>(from, allPieces) & ~friendlyPieces;

                while (attacks) {
                    int to = pop_lsb(attacks);

                    if ((1ULL << to) & enemyPieces) capture = true;
                    //((1ULL << to) & enemyPieces) ? capture = true : capture = false;
                    moves.emplace_back(from, to, piece, false, false, capture, false, false);
                    capture = false;
                }
            }
        }
    }

    template<PieceTypes pt>
    static BB attacksBitboard(int square, BB blockers) {
        static_assert(pt != PAWN && pt != KING, "Invalid piece type passed to attacks bitboard");
        switch (pt) {
            case QUEEN: return Magics::getQueenAttacks(square, blockers);
            case ROOK: return Magics::getRookAttacks(square, blockers);
            case BISHOP: return Magics::getBishopAttacks(square, blockers);
            case KNIGHT: return Bitboard::knightMoves[square];
        }
    }
};
#endif //CHESS_CPP_MOVEGEN_H

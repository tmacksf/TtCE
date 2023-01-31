//
// Created by Thomas Mack on 23/12/2022.
//

#ifndef CHESS_CPP_MOVEGEN_H
#define CHESS_CPP_MOVEGEN_H
#include "gameState.h"

class moveGen {
public:
    moveGen()= default;
    static void legalMoves(gameState gs, std::vector<Move> &moves);
    static void pseudoLegalMoves(gameState gs, std::vector<Move> &moves);

    // might want to make these inline to reduce function call overhead

    // leaping piece moves
    static void pawnMoves(int turn, BB pawns, BB allPieces, std::vector<Move> &moves);
    static void pawnAttacks(int turn, int enPassantSquare, BB pawns, BB allPieces, BB friendlyPieces, std::vector<Move> &moves);
    static void knightMoves(int turn, BB knights, BB friendlyPieces, BB enemyPieces, std::vector<Move> &moves);
    static void kingMoves(int turn, BB king, BB friendlyPieces, BB enemyPieces, std::vector<Move> &moves);

    // sliding attacks
    static void rookMoves();
    static void bishopMoves();
    static void queenMoves();

    // TODO Checks
};
#endif //CHESS_CPP_MOVEGEN_H

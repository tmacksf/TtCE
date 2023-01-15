//
// Created by Thomas Mack on 23/12/2022.
//

#include "moveGen.h"

/*
std::vector<gameState> *moveGen::legalMoves(gameState &gc) {

}
*/

void moveGen::pseudoLegalMoves(gameState gc, std::vector<Move> *moves) {
    gameState gsCopy = gc; // need to learn how to properly copy this
    int turn = gc.turn;
    BB allPieces = gc.allPieces();
    BB friendlyPieces = gc.friendlyBoard();
    BB enemyPieces = gc.enemyBoard();

    // we start with leaping piece moves. Pawn, knight, king
    int pawns = 5 + 6*turn;
    int pawnIndex = 0;
    while (gsCopy.bitboards[pawns].getValue()) {
        // checking the first pawn
        int shift = gsCopy.bitboards[pawns].getLeastSignificantBit();
        pawnIndex += shift; // shifting the bitboard down to get the next pawn then getting the index for move tables
        pawns >> shift;
        // getting the attack table for the pawn at the position
        BB attack = Bitboard::pawnAttacks[turn][pawnIndex];

        // getting the move table for the pawn at the position
        BB moves = Bitboard::pawnMoves[turn][shift];

        // unsetting the bit to do the next pawn
        gsCopy.bitboards[pawns].unSetBitAt(0);
    }
}
/*
void moveGen::generatePawnMoves(gameState gc) {

}*/
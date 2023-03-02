//
// Created by Thomas Mack on 23/12/2022.
//

#include "game.h"

using namespace std;

void initializeMoveGenerationInformation() {
    Bitboard::initAttackTables();
    Magics::generateSlidingAttackTables();
}

int Game(){
    gameState gs{};
    // need to do these two things before the game starts so need to make these a function and have it run at beginning
    /*gs.initialise("r3k2r/p2pqNb1/bnp1pnp1/3P4/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 2");

    //gs.initialise("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -");

    int total = 0;
    vector<Move> mv;
    moveGen::legalMoves(gs, mv);

    for (auto m :mv) m.printMove();
    for (auto m : moves) {
        vector<Move> moveVec;
        //m.printMove();
        gameState gsCopy = gs;
        gsCopy.makeMove(m);
        moveGen::legalMoves(gsCopy, moveVec);
        gsCopy.printing();
        for (auto mv: moveVec) mv.printMove();
        total += moveVec.size();
    }*/

    return 0;
}

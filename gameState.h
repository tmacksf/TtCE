//
// Created by Thomas Mack on 28/12/2022.
//

#ifndef CHESS_CPP_GAMESTATE_H
#define CHESS_CPP_GAMESTATE_H

#include "Bitboard.h"

class gameState {
public:
    Bitboard bitboards[12];

    void printing();
    void initialise(std::string fen);

};


#endif //CHESS_CPP_GAMESTATE_H

//
// Created by Thomas Mack on 28/12/2022.
//

#include <iostream>
#include "gameState.h"

/*gameState::gameState(std::string fen) {
    initialise(fen);
}*/

void gameState::initialise(std::string fen) {
    int i = 0;
    int boardPos = 0;
    for (i = 0; i < fen.length(); i++) {
        if (isnumber(fen[i])) {
            boardPos += fen[i];
        }
        else if (fen[i] == '/') {
            ;
        }
        else {

            boardPos += 1;
        }
    }
}
//
// Created by Tommy on 1/8/2023.
//

#ifndef CHESS_CPP_GAME_H
#define CHESS_CPP_GAME_H

#include "Search.hpp"

// Game will be the main function for interacting with
int Game();

int gameLoop(const std::string &startFen, Color engineColor);
void initInformation();
void parseUCIMove(std::string &move, int *moveLocation);
#endif // CHESS_CPP_GAME_H

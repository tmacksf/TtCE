//
// Created by Tommy on 1/8/2023.
//

#ifndef TTCE_GAME_HPP
#define TTCE_GAME_HPP

#include "Search.hpp"

namespace TtCE {

// Game will be the main function for interacting with
int Game();

int gameLoop(const std::string &startFen, Color engineColor);
void initInformation();
void parseUCIMove(std::string &move, int *moveLocation);
} // namespace TtCE
#endif // TTCE_GAME_HPP

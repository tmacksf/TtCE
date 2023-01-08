cmake_minimum_required(VERSION 3.23)
project(chess_cpp)

set(CMAKE_CXX_STANDARD 14)

add_executable(chess_cpp game.cpp gameState.cpp Bitboard.cpp Move.cpp Move.h main.cpp game.h)

cmake_minimum_required(VERSION 3.1)
project(TtCE)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

add_executable(chess_cpp game.cpp gameState.cpp Bitboard.cpp Move.hpp main.cpp moveGen.cpp Magics.cpp Testing.cpp Evaluation.hpp Search.hpp)

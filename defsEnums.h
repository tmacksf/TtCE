//
// Created by Tommy on 1/8/2023.
//

#ifndef CHESS_CPP_DEFSENUMS_H
#define CHESS_CPP_DEFSENUMS_H

#include <bitset>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <nmmintrin.h>
#include <random>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

// testing positions
#define STARTING_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define TRICKY_POSITION                                                        \
  "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - "
#define POSITION_3 "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - "
#define POSITION_4                                                             \
  "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"
#define POSITION_4_1                                                           \
  "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1 "
#define POSITION_5 "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8 "
#define POSITION_6                                                             \
  "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 "

constexpr unsigned long long nodeCount[6][8] = {
    {1, 20, 400, 8902, 197281, 4865609, 119060324, 3195901860},
    {1, 48, 2039, 97862, 4085603, 193690690, 8031647685, 0},
    {1, 14, 191, 2812, 43238, 674624, 11030083, 178633661},
    {1, 6, 264, 9467, 422333, 15833292, 706045033, 0},
    {1, 44, 1486, 62379, 2103487, 89941194, 0, 0},
    {1, 46, 2079, 89890, 3894594, 164075551, 6923051137, 0}};

typedef uint64_t BB;

constexpr int pieceCount = 12;

// all files, ranks, etc
const BB hFile = 0x0101010101010101ULL;
const BB gFile = hFile << 1;
const BB fFile = hFile << 2;
const BB eFile = hFile << 3;
const BB dFile = hFile << 4;
const BB cFile = hFile << 5;
const BB bFile = hFile << 6;
const BB aFile = hFile << 7;
const BB files[] = {hFile, gFile, fFile, eFile, dFile, cFile, bFile, aFile};

const BB rank8 = 0xFF00000000000000ULL;
const BB rank7 = rank8 >> 8;
const BB rank6 = rank7 >> 8;
const BB rank5 = rank6 >> 8;
const BB rank4 = rank5 >> 8;
const BB rank3 = rank4 >> 8;
const BB rank2 = rank3 >> 8;
const BB rank1 = rank2 >> 8;
const BB ranks[] = {rank1, rank2, rank3, rank4, rank5, rank6, rank7, rank8};

// diagonals
const BB rightLeft = 0x8040201008040201;
const BB leftRight = 0x0102040810204080;

const BB whiteSquares = 0xAA55AA55AA55AA55ULL;
const BB blackSquares = 0x55AA55AA55AA55AAULL;

const BB fullBoard = 0xFFFFFFFFFFFFFFFFULL;
const BB outerRing = 0x007E7E7E7E7E7E00ULL;

const int boardSize = 64;

// defining enums for directions and pieces
enum Color : int {
  WHITE = 0,
  BLACK = 1,
};

// Toggle color
constexpr Color operator~(Color c) { return Color(c ^ BLACK); }

// enum for files, don't need one for rank because it's a number already
/*enum Files : int {
    a, b, c, d, e, f, g, h
};*/

enum Board : int {
  // starting with h1 is zero
  h1,
  g1,
  f1,
  e1,
  d1,
  c1,
  b1,
  a1,
  h2,
  g2,
  f2,
  e2,
  d2,
  c2,
  b2,
  a2,
  h3,
  g3,
  f3,
  e3,
  d3,
  c3,
  b3,
  a3,
  h4,
  g4,
  f4,
  e4,
  d4,
  c4,
  b4,
  a4,
  h5,
  g5,
  f5,
  e5,
  d5,
  c5,
  b5,
  a5,
  h6,
  g6,
  f6,
  e6,
  d6,
  c6,
  b6,
  a6,
  h7,
  g7,
  f7,
  e7,
  d7,
  c7,
  b7,
  a7,
  h8,
  g8,
  f8,
  e8,
  d8,
  c8,
  b8,
  a8,
};

const std::string boardMap[]{
    "h1", "g1", "f1", "e1", "d1", "c1", "b1", "a1", "h2", "g2", "f2",
    "e2", "d2", "c2", "b2", "a2", "h3", "g3", "f3", "e3", "d3", "c3",
    "b3", "a3", "h4", "g4", "f4", "e4", "d4", "c4", "b4", "a4", "h5",
    "g5", "f5", "e5", "d5", "c5", "b5", "a5", "h6", "g6", "f6", "e6",
    "d6", "c6", "b6", "a6", "h7", "g7", "f7", "e7", "d7", "c7", "b7",
    "a7", "h8", "g8", "f8", "e8", "d8", "c8", "b8", "a8",
};

enum Direction : int {
  NORTH = 8,
  SOUTH = -NORTH,
  EAST = -1,
  WEST = -EAST,
  NORTH_EAST = NORTH + EAST,
  NORTH_WEST = NORTH + WEST,
  SOUTH_EAST = SOUTH + EAST,
  SOUTH_WEST = SOUTH + WEST
};

constexpr Direction pawn_push(Color c) { return c == BLACK ? SOUTH : NORTH; }

static const Direction allDirections[] = {
    NORTH, SOUTH, EAST, WEST, NORTH_EAST, NORTH_WEST, SOUTH_EAST, SOUTH_WEST};
static const Direction squareDirections[] = {NORTH, SOUTH, EAST, WEST};
static const Direction diagonalDirections[] = {NORTH_EAST, NORTH_WEST,
                                               SOUTH_WEST, SOUTH_EAST};

enum PieceTypes : int { KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN };

enum Pieces : int { K, Q, R, B, N, P, k, q, r, b, n, p };

enum MoveType : int {
  All = 1,
  Quiet,
  Captures,
  Evasions,
  PseudoLegal,
  Legal,
};

const char pieceToChar[] = {'K', 'Q', 'R', 'B', 'N', 'P',
                            'k', 'q', 'r', 'b', 'n', 'p'};

const std::string pieceToSymbolLight[] = {"♔", "♕", "♖", "♗", "♘", "♙",
                                          "♚", "♛", "♜", "♝", "♞", "♟︎"};

const std::string pieceToSymbolDark[] = {"♚", "♛", "♜", "♝", "♞", "♟︎",
                                         "♔", "♕", "♖", "♗", "♘", "♙"};
const int charToPiece[]{
    ['K'] = K, ['Q'] = Q, ['R'] = R, ['B'] = B, ['N'] = N, ['P'] = P,
    ['k'] = k, ['q'] = q, ['r'] = r, ['b'] = b, ['n'] = n, ['p'] = p};

const int charToFile[]{['a'] = 7, ['b'] = 6, ['c'] = 5, ['d'] = 4,
                       ['e'] = 3, ['f'] = 2, ['g'] = 1, ['h'] = 0};

// TODO use more system safe stuff
// Need to make sure this stuff can be run on multiple systems so may need to
// change the way this is done
inline int pop_lsb(BB &b) {
  assert(b);
  const int s = __builtin_ctzll(b);
  b &= b - 1;
  return s;
}

// for evaluation

constexpr int pieceValues[] = {10000,  9000,  500,  350,  300,  100,
                               -10000, -9000, -500, -350, -300, -100};

// CURRENT POSITION TABLES ARE FROM CHESS PROGRAMMING ON YOUTUBE
constexpr int pawnTable[] = {
    90, 90, 90, 90, 90, 90, 90, 90, 30, 30, 30, 40, 40, 30, 30, 30,
    20, 20, 20, 30, 30, 20, 20, 20, 10, 10, 10, 20, 20, 10, 10, 10,
    5,  5,  10, 20, 20, 10, 5,  5,  0,  0,  0,  5,  5,  0,  0,  0,
    0,  0,  0,  -5, -5, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};

constexpr int knightTable[] = {
    -5, 0,  0,  0,  0,  0,  0,  -5, -5, 0,   0,  10, 10, 0,  0,   -5,
    -5, 5,  20, 20, 20, 20, 5,  -5, -5, 10,  20, 30, 30, 20, 10,  -5,
    -5, 10, 20, 30, 30, 20, 10, -5, -5, 5,   20, 10, 10, 20, 5,   -5,
    -5, 0,  0,  0,  0,  0,  0,  -5, -5, -10, 0,  0,  0,  0,  -10, -5,
};

constexpr int bishopTable[] = {
    0, 0,  0,  0,  0,  0,  0,  0, 0, 0,  0,   0,  0,  0,   0,  0,
    0, 0,  0,  10, 10, 0,  0,  0, 0, 0,  10,  20, 20, 10,  0,  0,
    0, 0,  10, 20, 20, 10, 0,  0, 0, 10, 0,   0,  0,  0,   10, 0,
    0, 30, 0,  0,  0,  0,  10, 0, 0, 0,  -10, 0,  0,  -10, 0,  0,
};

constexpr int rookTable[] = {
    50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
    0,  0,  10, 20, 20, 10, 0,  0,  0,  0,  10, 20, 20, 10, 0,  0,
    0,  0,  10, 20, 20, 10, 0,  0,  0,  0,  10, 20, 20, 10, 0,  0,
    0,  0,  10, 20, 20, 10, 0,  0,  0,  0,  0,  20, 20, 0,  0,  0,
};

constexpr int queenTable[64] = {
    -28, 0,   29, 12,  59, 44, 43, 45, -24, -39, -5,  1,   -16, 57,  28,  54,
    -13, -17, 7,  8,   29, 56, 47, 57, -27, -27, -16, -16, -1,  17,  -2,  1,
    -9,  -26, -9, -10, -2, -4, 3,  -3, -14, 2,   -11, -2,  -5,  2,   14,  5,
    -35, -8,  11, 2,   8,  15, -3, 1,  -1,  -18, -9,  10,  -15, -25, -31, -50,
};

constexpr int kingTable[] = {
    0, 0, 0,  0,  0,  0,  0, 0, 0, 0, 5,  5,  5,   5,  0,  0,
    0, 5, 5,  10, 10, 5,  5, 0, 0, 5, 10, 20, 20,  10, 5,  0,
    0, 5, 10, 20, 20, 10, 5, 0, 0, 5, 5,  10, 10,  5,  5,  0,
    0, 5, 5,  -5, -5, 0,  5, 0, 0, 0, 5,  -5, -15, 0,  10, 0,
};

constexpr int mirrorScores[] = {
    a1, b1, c1, d1, e1, f1, g1, h1, a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3, a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5, a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7, a8, b8, c8, d8, e8, f8, g8, h8,
};

#define get_bit(b, i) ((b) & (1ULL << i))
static void printBitString(BB bitboard) {
  std::cout << "  a b c d e f g h" << std::endl;
  for (int file = 0; file < 8; file++) {
    std::cout << 8 - file;
    std::cout << " ";
    for (int rank = 0; rank < 8; rank++) {
      int square = 63 - (8 * file + rank);
      if (get_bit(bitboard, square))
        std::cout << 1;
      else
        std::cout << 0;
      std::cout << ' ';
    }
    std::cout << std::endl;
  }
}

// Search
constexpr int MAX_DEPTH = 128;
constexpr int INFINITE = 50000;
constexpr int MATE_VALUE = 49000;
constexpr int MATE_SCORE = 48000;

// Hashing
constexpr int hashFlagExact = 0;
constexpr int hashFlagAlpha = 1;
constexpr int hashFlagBeta = 2;

// around 5 MB
constexpr unsigned long transpositionTableSize = 0x500000;
constexpr int valUnknown = 100000;

#if defined(__WIN32__)

#include <windows.h>

int64_t time_ms(void) { return timeGetTime(); }

#else

#include <sys/time.h>

static inline int64_t time_ms(void) {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (int64_t)tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

#endif

#endif // CHESS_CPP_DEFSENUMS_H

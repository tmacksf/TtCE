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

// TODO: Make this the namespace for the whole project
namespace ttce {}
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

// debugging
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

namespace Eval {
// enum for the phase of the game
enum Phase { Opening, Endgame, Middlegame };

constexpr int OPENINGSCORE = 6192;
constexpr int ENDGAMESCORE = 518;

// for mapping black pieces to the white piece types value
constexpr PieceTypes PieceMapping[] = {
    KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN,
    KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN,
};

// evaluation numbers

// material
constexpr int materialScore[2][12] = {
    // opening material score
    {12000, 1025, 477, 365, 337, 82, -12000, -1025, -477, -365, -337, -82},

    // endgame material score
    {12000, 936, 512, 297, 281, 94, -12000, -936, -512, -297, -281, -94},
};

// Positional
constexpr int positionalScores[2][6][64] = {
    // opening positional piece scores //
    {
        // king
        {
            -65, 23,  16,  -15, -56, -34, 2,   13,  29,  -1,  -20, -7,  -8,
            -4,  -38, -29, -9,  24,  2,   -16, -20, 6,   22,  -22, -17, -20,
            -12, -27, -30, -25, -14, -36, -49, -1,  -27, -39, -46, -44, -33,
            -51, -14, -14, -22, -46, -44, -30, -15, -27, 1,   7,   -8,  -64,
            -43, -16, 9,   8,   -15, 36,  12,  -54, 8,   -28, 24,  14,
        },
        // queen
        {
            -28, 0,   29, 12,  59,  44,  43, 45,  -24, -39, -5,  1,   -16,
            57,  28,  54, -13, -17, 7,   8,  29,  56,  47,  57,  -27, -27,
            -16, -16, -1, 17,  -2,  1,   -9, -26, -9,  -10, -2,  -4,  3,
            -3,  -14, 2,  -11, -2,  -5,  2,  14,  5,   -35, -8,  11,  2,
            8,   15,  -3, 1,   -1,  -18, -9, 10,  -15, -25, -31, -50,
        },
        // rook
        {
            32,  42,  32,  51,  63,  9,   31,  43,  27,  32,  58,  62,  80,
            67,  26,  44,  -5,  19,  26,  36,  17,  45,  61,  16,  -24, -11,
            7,   26,  24,  35,  -8,  -20, -36, -26, -12, -1,  9,   -7,  6,
            -23, -45, -25, -16, -17, 3,   0,   -5,  -33, -44, -16, -20, -9,
            -1,  11,  -6,  -71, -19, -13, 1,   17,  16,  7,   -37, -26,
        },
        // bishop
        {
            -29, 4,  -82, -37, -25, -42, 7,   -8,  -26, 16,  -18, -13, 30,
            59,  18, -47, -16, 37,  43,  40,  35,  50,  37,  -2,  -4,  5,
            19,  50, 37,  37,  7,   -2,  -6,  13,  13,  26,  34,  12,  10,
            4,   0,  15,  15,  15,  14,  27,  18,  10,  4,   15,  16,  0,
            7,   21, 33,  1,   -33, -3,  -14, -21, -13, -12, -39, -21,
        },
        // knight
        {
            -167, -89, -34, -49, 61,   -97, -15, -107, -73, -41, 72,  36,  23,
            62,   7,   -17, -47, 60,   37,  65,  84,   129, 73,  44,  -9,  17,
            19,   53,  37,  69,  18,   22,  -13, 4,    16,  13,  28,  19,  21,
            -8,   -23, -9,  12,  10,   19,  17,  25,   -16, -29, -53, -12, -3,
            -1,   18,  -14, -19, -105, -21, -58, -33,  -17, -28, -19, -23,
        },
        // pawn
        {
            0,   0,   0,   0,   0,   0,   0,   0,  98,  134, 61,  95,  68,
            126, 34,  -11, -6,  7,   26,  31,  65, 56,  25,  -20, -14, 13,
            6,   21,  23,  12,  17,  -23, -27, -2, -5,  12,  17,  6,   10,
            -25, -26, -4,  -4,  -10, 3,   3,   33, -12, -35, -1,  -20, -23,
            -15, 24,  38,  -22, 0,   0,   0,   0,  0,   0,   0,   0,
        },
    },
    {// Endgame positional piece scores //
     // king
     {-74, -35, -18, -18, -11, 15,  4,   -17, -12, 17,  14,  17, 17,
      38,  23,  11,  10,  17,  23,  15,  20,  45,  44,  13,  -8, 22,
      24,  27,  26,  33,  26,  3,   -18, -4,  21,  24,  27,  23, 9,
      -11, -19, -3,  11,  21,  23,  16,  7,   -9,  -27, -11, 4,  13,
      14,  4,   -5,  -17, -53, -34, -21, -11, -28, -14, -24, -43},
     // queen
     {
         -9,  22,  22,  27,  27,  19,  10,  20,  -17, 20,  32,  41,  58,
         25,  30,  0,   -20, 6,   9,   49,  47,  35,  19,  9,   3,   22,
         24,  45,  57,  40,  57,  36,  -18, 28,  19,  47,  31,  34,  39,
         23,  -16, -27, 15,  6,   9,   17,  10,  5,   -22, -23, -30, -16,
         -16, -23, -36, -32, -33, -28, -22, -43, -5,  -32, -20, -41,
     },
     // rook
     {
         13, 10, 18, 15, 12, 12, 8,   5,   11, 13, 13, 11, -3, 3,   8,  3,
         7,  7,  7,  5,  4,  -3, -5,  -3,  4,  3,  13, 1,  2,  1,   -1, 2,
         3,  5,  8,  4,  -5, -6, -8,  -11, -4, 0,  -5, -1, -7, -12, -8, -16,
         -6, -6, 0,  2,  -9, -9, -11, -3,  -9, 2,  3,  -1, -5, -13, 4,  -20,
     },
     // bishop
     {
         -14, -21, -11, -8,  -7,  -9, -17, -24, -8,  -4,  7,   -12, -3,
         -13, -4,  -14, 2,   -8,  0,  -1,  -2,  6,   0,   4,   -3,  9,
         12,  9,   14,  10,  3,   2,  -6,  3,   13,  19,  7,   10,  -3,
         -9,  -12, -3,  8,   10,  13, 3,   -7,  -15, -14, -18, -7,  -1,
         4,   -9,  -15, -27, -23, -9, -23, -5,  -9,  -16, -5,  -17,
     },
     // knight
     {
         -58, -38, -13, -28, -31, -27, -63, -99, -25, -8,  -25, -2,  -9,
         -25, -24, -52, -24, -20, 10,  9,   -1,  -9,  -19, -41, -17, 3,
         22,  22,  22,  11,  8,   -18, -18, -6,  16,  25,  16,  17,  4,
         -18, -23, -3,  -1,  15,  10,  -3,  -20, -22, -42, -20, -10, -5,
         -2,  -20, -23, -44, -29, -51, -23, -15, -22, -18, -50, -64,
     },
     // pawn
     {
         0,   0,   0,   0,  0,   0,  0,  0,  178, 173, 158, 134, 147,
         132, 165, 187, 94, 100, 85, 67, 56, 53,  82,  84,  32,  24,
         13,  5,   -2,  4,  17,  17, 13, 9,  -3,  -7,  -7,  -8,  3,
         -1,  4,   7,   -6, 1,   0,  -5, -1, -8,  13,  8,   8,   10,
         13,  0,   2,   -7, 0,   0,  0,  0,  0,   0,   0,   0,
     }}};

// values
constexpr int mirrorScores[] = {
    a1, b1, c1, d1, e1, f1, g1, h1, a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3, a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5, a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7, a8, b8, c8, d8, e8, f8, g8, h8,
};
}; // namespace Eval

#endif // CHESS_CPP_DEFSENUMS_H

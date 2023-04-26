//
// Created by Thomas Mack on 04/02/2023.
//

#include "Testing.h"
#include "Evaluation.h"
#include "defsEnums.h"
#include "gameState.h"
#include <cstdint>
#include <ctime>
#include <sys/_types/_int64_t.h>
#include <typeinfo>
using namespace std;

int Testing::quickTests() {
  gameState gs;
  gameState gsGood;
  // gs.initialise("rnbqkbnr/ppppppp1/7p/8/8/7P/PPPPPPP1/RNBQKBNR w KQkq - 0
  // 1");
  // gs.initialise("8/p1pR2pp/2k2b2/1p6/1PN2p2/7P/5PPK/1r6 b - - 0 1");
  gs.initialise("5k2/6p1/6Qp/7P/P3P3/1P4PK/2r2r2/8 w - - 9 44");
  cout << Evaluation::evaluate(gs);
  Search s;

  // allPerftTests();

  // s.findBestMove(gs, 25, 30000);

  return 0;
}

int Testing::AllTests() {
  quickTests();

  // BB hash = gs.getHash();
  // cout << "\nNot passed on: " << hashTesting(gs, 5, hash);
  // perftTest(gs, perftDepth);
  return 0;
}

void Testing::perftTest(gameState &gs, int perftDepth) {
  BB nodeCount = 0;
  auto start = chrono::steady_clock::now();
  vector<Move> moves;
  moveGen::legalMoves<All>(gs, moves);
  for (auto m : moves) {
    int currentIter = 0;
    gameState gsCopy = gs;
    gsCopy.makeMove(m, 0ULL);
    currentIter = Perft(gsCopy, perftDepth - 1);
    cout << "     ";
    m.printMove();
    cout << "   Nodes: " << currentIter << "\n";
    nodeCount += currentIter;
  }
  auto end = chrono::steady_clock::now();
  auto diff = end - start;

  cout << "     Depth: " << perftDepth << "\n     Nodes: " << nodeCount
       << "\n     Time: " << chrono::duration<double, milli>(diff).count()
       << endl;
}

BB Testing::Perft(gameState &gs, int depth) {
  std::vector<Move> moves;
  moves.reserve(32);
  moveGen::legalMoves<All>(gs, moves);
  // moveGen::mg2(gs, moves);

  if (depth == 1)
    return moves.size();

  BB nodes = 0;
  for (Move &move : moves) {
    gameState gsCopy = gs;
    gsCopy.makeMove(move, 0ULL);
    nodes += Perft(gsCopy, depth - 1);
  }
  return nodes;
}

BB Testing::PerftBrokenUp(gameState &gs, int depth) {
  std::vector<Move> moves;
  moves.reserve(32);
  moveGen::legalMoves<Captures>(gs, moves);
  moveGen::legalMoves<Quiet>(gs, moves);
  // moveGen::mg2(gs, moves);

  if (depth == 1)
    return moves.size();

  BB nodes = 0;
  for (Move &move : moves) {
    gameState gsCopy = gs;
    gsCopy.makeMove(move, 0ULL);
    nodes += Perft(gsCopy, depth - 1);
  }
  return nodes;
}

BB Testing::PerftAdvanced(gameState &gs, int depth, int *extraInfo) {
  // extra info contains information about ep, captures, castling,
  // double push etc. Order: Captures, E.P, Castle, Promotions
  std::vector<Move> moves;
  // moveGen::legalMoves(gs, moves);
  moveGen::legalMoves<All>(gs, moves);

  if (depth == 1) {
    for (Move move : moves) {
      if (move.m_captureFlag)
        extraInfo[0] += 1;
      if (move.m_enPassantFlag)
        extraInfo[1] += 1;
      if (move.m_castleFlag)
        extraInfo[2] += 1;
      if (move.m_promotedPiece)
        extraInfo[3] += 1;
    }
    return moves.size();
  }
  BB nodes = 0;
  for (Move move : moves) {
    gameState gsCopy = gs;
    gsCopy.makeMove(move, 0ULL);
    nodes += PerftAdvanced(gsCopy, depth - 1, extraInfo);
  }
  return nodes;
}

BB Testing::advancedPerftDriver(gameState &gs, int depth) {
  int extraInfo[] = {0, 0, 0, 0};
  BB moveCount = 0;
  moveCount += PerftAdvanced(gs, depth, extraInfo);

  cout << "Move count: " << moveCount << " Captures: " << extraInfo[0]
       << " E.P. " << extraInfo[1] << " Castles: " << extraInfo[2]
       << " Promotions: " << extraInfo[3] << endl;

  return moveCount;
}

int Testing::hashTesting(gameState &gs, int depth, BB hash) {
  std::vector<Move> moves;
  moves.reserve(32);
  moveGen::legalMoves<All>(gs, moves);

  if (depth == 1)
    return 0;

  BB nodes = 0;
  for (Move &move : moves) {
    gameState gsCopy = gs;
    BB _hash = gsCopy.makeMove(move, hash);
    BB workingHash = gsCopy.getHash();

    if (_hash != workingHash) {
      move.printMove(1);
      cout << endl;
      gsCopy.printing();
      gsCopy.stateToFen();
      cout << "Working hash " << workingHash << " New hash " << _hash;
      return 1;
    }

    nodes += hashTesting(gsCopy, depth - 1, _hash);
  }
  return nodes;
}

void Testing::allPerftTests() {
  gameState gs;
  gs.initialise(STARTING_FEN);
  int testsPassed = 0;

  int nodes = 0;
  nodes = Perft(gs, 6);
  if (nodes == nodeCount[0][6]) {
    testsPassed += 1;
    cout << "Passed 1, Nodes: " << nodes << endl;
  }
  gs.clear();

  gs.initialise(TRICKY_POSITION);
  nodes = 0;
  nodes = Perft(gs, 5);
  if (nodes == nodeCount[1][5]) {
    testsPassed += 1;
    cout << "Passed 2, Nodes: " << nodes << endl;
  }
  gs.clear();

  gs.initialise(POSITION_3);
  nodes = 0;
  nodes = Perft(gs, 6);
  if (nodes == nodeCount[2][6]) {
    testsPassed += 1;
    cout << "Passed 3, Nodes: " << nodes << endl;
  }
  gs.clear();

  gs.initialise(POSITION_4);
  nodes = 0;
  nodes = Perft(gs, 6);
  if (nodes == nodeCount[3][6]) {
    testsPassed += 1;
    cout << "Passed 4, Nodes: " << nodes << endl;
  }
  gs.clear();

  gs.initialise(POSITION_4_1);
  nodes = 0;
  nodes = Perft(gs, 6);
  if (nodes == nodeCount[3][6]) {
    testsPassed += 1;
    cout << "Passed 4.1, Nodes: " << nodes << endl;
  }
  gs.clear();

  gs.initialise(POSITION_5);
  nodes = 0;
  nodes = Perft(gs, 5);
  if (nodes == nodeCount[4][5]) {
    testsPassed += 1;
    cout << "Passed 5, Nodes: " << nodes << endl;
    ;
  }
  gs.clear();

  gs.initialise(POSITION_6);
  nodes = 0;
  nodes = Perft(gs, 5);
  if (nodes == nodeCount[5][5]) {
    testsPassed += 1;
    cout << "Passed 6, Nodes: " << nodes << endl;
  }

  if (testsPassed == 7) {
    cout << "All passed" << endl;
  } else {
    cout << "Failed" << endl;
  }
}

//
// Created by Thomas Mack on 04/02/2023.
//

#include "Testing.hpp"
#include "defsEnums.hpp"
using namespace std;

namespace TtCE {

int Testing::quickTests() {
  gameState gs;
  gameState gsGood;
  // gs.initialise(
  //"r3k2r/p1ppqpb1/1n2pnp1/3PN3/1p2P3/2N2Q1p/PPPB1PPP/R3K2R w KQkq - 0 1");
  gs.initialise(STARTING_FEN);
  gs.printing();
  Search s;
  // s.testingFindBestMove(gs, 7);
  cout << Eval::Evaluation::evaluate(gs) << endl;

  printBitString(Bitboard::kingMoves[e4]);

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
  // moveGenTesting::legalMoves<All>(gs, moves);

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

  auto start = chrono::steady_clock::now();
  int nodes = 0;
  nodes = Perft(gs, 6);
  auto end = chrono::steady_clock::now();
  auto diff = end - start;
  if (nodes == nodeCount[0][6]) {
    testsPassed += 1;
    cout << "Passed 1, Nodes: " << nodes
         << " Time: " << chrono::duration<double, milli>(diff).count() << endl;
  }
  gs.clear();

  gs.initialise(TRICKY_POSITION);
  nodes = 0;
  start = chrono::steady_clock::now();
  nodes = Perft(gs, 5);
  end = chrono::steady_clock::now();
  diff = end - start;
  if (nodes == nodeCount[1][5]) {
    testsPassed += 1;
    cout << "Passed 2, Nodes: " << nodes
         << " Time: " << chrono::duration<double, milli>(diff).count() << endl;
  }
  gs.clear();

  gs.initialise(POSITION_3);
  nodes = 0;
  start = chrono::steady_clock::now();
  nodes = Perft(gs, 6);
  end = chrono::steady_clock::now();
  diff = end - start;
  if (nodes == nodeCount[2][6]) {
    testsPassed += 1;
    cout << "Passed 3, Nodes: " << nodes
         << " Time: " << chrono::duration<double, milli>(diff).count() << endl;
  }
  gs.clear();

  gs.initialise(POSITION_4);
  nodes = 0;
  start = chrono::steady_clock::now();
  nodes = Perft(gs, 6);
  end = chrono::steady_clock::now();
  diff = end - start;
  if (nodes == nodeCount[3][6]) {
    testsPassed += 1;
    cout << "Passed 4, Nodes: " << nodes
         << " Time: " << chrono::duration<double, milli>(diff).count() << endl;
  }
  gs.clear();

  gs.initialise(POSITION_4_1);
  nodes = 0;
  start = chrono::steady_clock::now();
  nodes = Perft(gs, 6);
  end = chrono::steady_clock::now();
  diff = end - start;
  if (nodes == nodeCount[3][6]) {
    testsPassed += 1;
    cout << "Passed 5, Nodes: " << nodes
         << " Time: " << chrono::duration<double, milli>(diff).count() << endl;
  }
  gs.clear();

  gs.initialise(POSITION_5);
  nodes = 0;
  start = chrono::steady_clock::now();
  nodes = Perft(gs, 5);
  end = chrono::steady_clock::now();
  diff = end - start;
  if (nodes == nodeCount[4][5]) {
    testsPassed += 1;
    cout << "Passed 6, Nodes: " << nodes
         << " Time: " << chrono::duration<double, milli>(diff).count() << endl;
  }
  gs.clear();

  gs.initialise(POSITION_6);
  nodes = 0;
  start = chrono::steady_clock::now();
  nodes = Perft(gs, 5);
  end = chrono::steady_clock::now();
  diff = end - start;
  if (nodes == nodeCount[5][5]) {
    testsPassed += 1;
    cout << "Passed 7, Nodes: " << nodes
         << " Time: " << chrono::duration<double, milli>(diff).count() << endl;
  }

  if (testsPassed == 7) {
    cout << "All passed" << endl;
  } else {
    cout << "Failed" << endl;
  }
}
} // namespace TtCE

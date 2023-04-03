//
// Created by Thomas Mack on 04/02/2023.
//

#include "Testing.h"
#include "defsEnums.h"
#include <algorithm>
using namespace std;

int Testing::quickTests() {
  gameState gs;
  gs.initialise(
      "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
  // gs.initialise("6k1/3q1pp1/pp5p/1r5n/8/1P3PP1/PQ4BP/2R3K1 w - - 0 1");
  gs.printing();

  Search s;
  s.findBestMove(gs, 8);
  s.getBestMove().printMove();
  cout << "\nNode count: " << s.getNodes()
       << " Q node count: " << s.getQNodes();

  return 0;
}

int Testing::AllTests() {
  int perftDepth = 6;
  gameState gs{};
  // gs.initialise(STARTING_FEN);
  gs.initialise("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 "
                "w - - 0 10 ");
  // perftTest(gs, perftDepth, 1);

  quickTests();
  return 0;
}

void Testing::perftTest(gameState &gs, int perftDepth, int debug) {
  BB nodeCount;
  auto start = chrono::steady_clock::now();
  nodeCount = Perft(gs, perftDepth, debug);
  //  nodeCount = advancedPerftDriver(gs, perftDepth);
  auto end = chrono::steady_clock::now();
  auto diff = end - start;

  cout << nodeCount
       << " Time: " << chrono::duration<double, milli>(diff).count() << endl;
}

BB Testing::Perft(gameState &gs, int depth, int debug) {
  std::vector<Move> moves;
  moves.reserve(32);
  if (debug)
    moveGen::mg2(gs, moves);
  else
    moveGen::legalMoves<All>(gs, moves);

  if (depth == 1)
    return moves.size();
  BB nodes = 0;
  for (Move move : moves) {
    gameState gsCopy = gs;
    gsCopy.makeMove(move);
    nodes += Perft(gsCopy, depth - 1, debug);
  }
  return nodes;
}

BB Testing::PerftAdvanced(gameState &gs, int depth, int *extraInfo) {
  // extra info contains information about ep, captures, castling, double push
  // etc. Order: Captures, E.P, Castle, Promotions
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
    gsCopy.makeMove(move);
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

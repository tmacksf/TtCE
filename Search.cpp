//
// Created by Thomas Mack on 08/03/2023.
//

#include "Search.h"
#include "defsEnums.h"

BB Search::zArray[12][64];
BB Search::zEP[8];
BB Search::zCastle[4];
BB Search::zBlackMove;

void Search::initZobrist() {
  std::random_device rd;
  std::mt19937_64 eng(rd());
  std::uniform_int_distribution<unsigned long long> distr;
  for (int i = 0; i < 12; i++) {
    for (int j = 0; j < 64; j++) {
      zArray[i][j] = distr(eng);
    }
  }
  for (int i = 0; i < 8; i++) {
    zEP[i] = distr(eng);
  }
  for (int i = 0; i < 4; i++) {
    zCastle[i] = distr(eng);
  }
  zBlackMove = distr(eng);
}

BB Search::getHash(const gameState &gs) {
  BB zHash = 0ULL;
  BB pieces = gs.allPieces();
  int square;
  int pt;
  // piece hashing
  while (pieces) {
    square = pop_lsb(pieces);
    pt = gs.getPieceAt(square);
    zHash ^= zArray[pt][square];
  }
  // EP hashing by column (Get double push square)
  // Not sure if this is necessary
  if (gs.getEPSquare() != -1)
    zHash ^= zEP[gs.getEPSquare() % 8];

  // castling
  for (BB zk : zCastle) {
    zHash ^= zk;
  }

  if (gs.getTurn())
    zHash ^= zBlackMove;

  return zHash;
}

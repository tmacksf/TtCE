//
// Created by Tommy on 1/8/2023.
//
#include "Testing.h"

using namespace std;

// static tables are initialised here
Move Move::killerMove[2][MAX_DEPTH];
int Move::historyMove[12][64];

int main() {
  Search::initEngine();

  bool debug = false;
  // debug = true;

  if (debug)
    Testing::AllTests();
  else {
    UCI uci;
    uci.UCILoop();
  }
  return 0;
}

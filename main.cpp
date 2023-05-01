//
// Created by Tommy on 1/8/2023.
//
#include "Testing.hpp"

using namespace std;
// static tables are initialised here
TtCE::Move TtCE::Move::killerMove[2][MAX_DEPTH];
int TtCE::Move::historyMove[12][64];

int main() {
  TtCE::Search::initEngine();

  bool debug = false;
  debug = true;

  if (debug)
    TtCE::Testing::AllTests();
  else {
    TtCE::UCI uci;
    uci.UCILoop();
  }
  return 0;
}

#include "Testing.h"

using namespace std;

class UCI {
private:
  Search s{};

public:
  Move parseUCIMove() {}
  void parsePosition() {}
  void parseGo() {}
  void UCILoop() {
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    char input[300];

    cout << "id name Thomas the Chess Engine\n";
    cout << "uciok";
  }
};

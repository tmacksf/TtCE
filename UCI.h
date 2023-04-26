#include "Search.h"
#include "defsEnums.h"
#include <sstream>
#include <stdexcept>

using namespace std;

class UCI {
private:
  string engineName = "Thomas the Chess Engine";
  gameState m_gs;
  Search m_engine;

public:
  gameState *getGs() { return &m_gs; }

  template <typename T> T getNumberFromString(const string &s) {
    T t = T();
    stringstream ss(s);
    ss >> t;
    return t;
  }

  int parseUCIMove(std::string move) {
    int fromSquare;
    int toSquare;

    fromSquare = charToFile[move[0]] + (8 * (int(move[1]) - '1'));
    toSquare = charToFile[move[2]] + (8 * (int(move[3]) - '1'));

    vector<Move> moves;
    moveGen::legalMoves<All>(m_gs, moves);

    int promotedPiece;
    int addition = m_gs.getTurn() * 6;
    if (move.length() == 5) {
      if (move[4] == 'q') {
        promotedPiece = QUEEN + addition;
      } else if (move[4] == 'r') {
        promotedPiece = ROOK + addition;
      } else if (move[4] == 'b') {
        promotedPiece = BISHOP + addition;
      } else if (move[4] == 'k') {
        promotedPiece = KNIGHT + addition;
      }

      for (Move m : moves) {
        if (m.m_fromSquare == fromSquare && m.m_toSquare == toSquare &&
            promotedPiece == m.m_promotedPiece) {
          BB hash = m_gs.getHash();
          cout << promotedPiece << endl;
          m_gs.makeMove(m, hash);
          return 0;
        }
      }
    } else {
      for (Move m : moves) {
        if (m.m_fromSquare == fromSquare && m.m_toSquare == toSquare) {
          BB hash = m_gs.getHash();
          m_gs.makeMove(m, hash);
          return 0;
        }
      }
    }
    return -1;
  }

  void parseUCIMoves(string &str) {
    while (str.size()) {
      int spaceLoc = str.find(" ");
      if (spaceLoc == 0) {
        // removing first space from the string
        str.erase(0, 1);
      }
      if (spaceLoc == -1) {
        parseUCIMove(str);
        break;
      } else {
        const string temp = str.substr(0, spaceLoc);
        cout << temp << endl;
        parseUCIMove(temp);
        str.erase(0, spaceLoc);
      }
    }
  }

  void moveToFen(const Move &m) {}

  void parsePosition(const string &str) {
    int moveLoc = str.find("moves");

    if (str.find("startpos") != string::npos) {
      m_gs.initialise(STARTING_FEN);
    }

    else if (str.find("fen") == string::npos) {
      m_gs.initialise(STARTING_FEN);
    }

    else {
      if (moveLoc == string::npos) {
        m_gs.initialise(str.substr(4, str.size() - 4));
      } else {
        m_gs.initialise(str.substr(4, moveLoc - 4));
        // cout << str.substr(4, moveLoc - 4) << endl;
      }
    }
    if (moveLoc != string::npos) {
      string allMoves = str.substr(moveLoc + 6);
      parseUCIMoves(allMoves);
    }
  }

  void parseGo(const string &str) {
    int movesToGo = 30;
    int movetime = -1;
    int time = -1;
    int inc = 0;
    int depth = 25;
    std::cout << str << endl;

    if (str.find("infinite") == 0) {
      throw runtime_error("TODO Infinite");
      // TODO: This
    }

    else if (str.find("ponder") == 0) {
      throw runtime_error("TODO ponder");
      // TODO: This
    }

    if (str.find("depth") < 0) {
      size_t last_index = str.find_last_not_of("0123456789");
      string result = str.substr(last_index + 1);
      depth = stoi(result);
    }

    // TODO: Make sure that the time it gets is correct
    if (str.find("wtime") < 150 && !m_gs.getTurn()) {
      int intstart = str.find("wtime") + 6;

      vector<int> v;
      stringstream ss(str.substr(intstart));
      string line;
      while (getline(ss, line, ' ')) {
        v.push_back(getNumberFromString<int>(line));
      }

      int time = v[0];
      int movesToGo = 40; // default for if this isn't sent to engine
      int mtwogolocation = str.find("movestogo");
      vector<int> v2;
      if (mtwogolocation < 100) {
        stringstream ss(str.substr(mtwogolocation + 10));
        string line;
        while (getline(ss, line, ' ')) {
          v2.push_back(getNumberFromString<int>(line));
        }

        movesToGo = v2.back();
      }

      time = time / movesToGo - 5; // little barrier of 5 ms for extra
      cout << time << endl;

      gameState gs = m_gs;
      m_engine.findBestMove(gs, 25, time);
    }

    else if (str.find("btime") < 100 && m_gs.getTurn()) {
      int intstart = str.find("btime") + 6;

      vector<int> v;
      stringstream ss(str.substr(intstart));
      string line;
      while (getline(ss, line, ' ')) {
        v.push_back(getNumberFromString<int>(line));
      }

      int time = v[0];
      int movesToGo = 40; // default for if this isn't sent to engine
      int mtwogolocation = str.find("movestogo");
      vector<int> v2;
      if (mtwogolocation < 100) {
        stringstream ss(str.substr(mtwogolocation + 10));
        string line;
        while (getline(ss, line, ' ')) {
          v2.push_back(getNumberFromString<int>(line));
        }

        movesToGo = v2.back();
      }

      time = time / movesToGo - 5; // little barrier of 5 ms for extra
      cout << "time: " << time << endl;

      gameState gs = m_gs;
      m_engine.findBestMove(gs, 25, time);
    }

    else if (str.find("nodes") < 150) {
      // TODO: this
      throw runtime_error("TODO nodes");
    }

    else if (str.find("movetime") < 150) {
      int intstart = str.find("movetime") + 9;

      vector<int> v;
      stringstream ss(str.substr(intstart));
      string line;
      while (getline(ss, line, ' ')) {
        v.push_back(getNumberFromString<int>(line));
      }
      cout << v.back() << endl;

      gameState gs = m_gs;
      m_engine.findBestMove(gs, 25, v.back());
    }

    else {
      // in the case where only depth is specified
      gameState gs = m_gs;
      m_engine.findBestMove(gs, depth, 0);
    }
  }
  void UCILoop() {
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    string input = "";

    cout << "id name " << engineName << "\n";
    cout << "uciok\n";

    while (true) {
      input = "";

      getline(cin, input); // command

      if (input.compare("isready") == 0) {
        cout << "readyok\n";
        continue;
      }

      else if (input.compare(0, 8, "position") == 0) {
        m_gs.clear();
        parsePosition(input.substr(9));
      }

      else if (input.compare(0, 10, "ucinewgame") == 0) {
        m_gs.clear();
        m_gs.initialise(STARTING_FEN);
      }

      else if (input.compare(0, 2, "go") == 0) {
        parseGo(input.substr(3));
      }

      else if (input.compare(0, 4, "quit") == 0) {
        break;
      }

      else if (input.compare(0, 4, "stop") == 0) {
        // TODO: break out of current search (threading)
        break;
      }

      else if (input.compare(0, 3, "uci") == 0) {
        cout << "id name " << engineName << "\n";
        cout << "uciok\n";
      }

      else if (input.compare(0, 5, "print") == 0) {
        m_gs.printing();
      }
    }
  }
};

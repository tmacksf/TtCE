//
// Created by Thomas Mack on 23/12/2022.
//

#include "game.h"
#include "Evaluation.h"
#include "defsEnums.h"

using namespace std;

void initializeMoveGenerationInformation() {
  Bitboard::initAttackTables();
  Magics::generateSlidingAttackTables();
  Search::initZobrist();
}

Move Move::killerMove[2][MAX_DEPTH];
int Move::historyMove[12][64];

int Game() {
  int status = 0;

  int turn;
  cout << "Select turn (0 for white and 1 for black): ";
  cin >> turn;

  if (turn)
    status = gameLoop(STARTING_FEN, BLACK);
  else
    status = gameLoop(STARTING_FEN, WHITE);

  return status;
}

void parseUCIMove(std::string &move, int *moveLocation) {
  int fromSquare;
  int toSquare;

  fromSquare = charToFile[move[0]] + (8 * (int(move[1]) - '1'));
  toSquare = charToFile[move[2]] + (8 * (int(move[3]) - '1'));

  // TODO add more clarification about promoted piece. If it is whites turn for
  // example
  if (move.length() == 5) {
    switch (move[4]) {
    case 'q':
      moveLocation[2] = q;
    case 'Q':
      moveLocation[2] = Q;
    case 'r':
      moveLocation[2] = r;
    case 'R':
      moveLocation[2] = R;
    case 'b':
      moveLocation[2] = b;
    case 'B':
      moveLocation[2] = B;
    case 'n':
      moveLocation[2] = n;
    case 'N':
      moveLocation[2] = N;
    }
  }
  moveLocation[0] = fromSquare;
  moveLocation[1] = toSquare;
}

Move isMoveLegal(const vector<Move> &moves, const int *moveLocation) {
  for (int i = 0; i < moves.size(); i++) {
    if (moves.at(i).m_fromSquare == moveLocation[0] &&
        moves.at(i).m_toSquare == moveLocation[1] &&
        moves.at(i).m_promotedPiece == moveLocation[2]) {
      return moves.at(i);
    }
  }
  return Move{-1, 0};
}

void playerTurn(int *moveLocation) {
  string move;

  cin >> move;
  parseUCIMove(move, moveLocation);
  cout << moveLocation[0] << moveLocation[1] << moveLocation[2] << endl;
}

Move engineTurn(const gameState &gs) {

  Search negamaxSearch{};
  negamaxSearch.findBestMove(gs, 7);

  cout << "\nEngine move: ";
  negamaxSearch.getBestMove().printMove();
  cout << endl;

  return negamaxSearch.getBestMove();
}

int gameLoop(const string &startFen, Color engineColor) {
  Color playerColor = ~engineColor;
  gameState gs{}; // current board state
  gs.initialise(startFen);
  bool hasWon = false;
  while (!hasWon) {
    // Print current game state
    gs.printing();
    // loop to make sure move is legal

    if (gs.getTurn() == engineColor) {
      gs.makeMove(engineTurn(gs));
    } else {
      std::vector<Move> legalMoves;
      moveGen::legalMoves<All>(gs, legalMoves);
      bool validMove = false;
      while (!validMove) {
        int moveLocation[] = {0, 0, 0};
        playerTurn(moveLocation);
        Move m = isMoveLegal(legalMoves, moveLocation);
        cout << m.m_fromSquare << m.m_toSquare << endl;
        if (m.m_fromSquare != -1) {
          validMove = true;
          gs.makeMove(m);
        } else {
          cout << "Move was invalid. Please enter again.\n";
          for (Move m : legalMoves) {
            m.printMove();
          }
        }
      }
    }

    vector<Move> moves;
    moveGen::legalMoves<All>(gs, moves);
    if (gs.isKingInCheck(gs.getAttacking()) && !moves.size()) {
      hasWon = true;
      gs.printing();
      // need to reverse these as it checks after the move is made
      if (gs.getTurn())
        cout << "White wins!";
      else
        cout << "Black wins!";
    } else if (!moves.size()) {
      hasWon = true;
      gs.printing();
      cout << "Tie" << endl;
    }
  }
  return 0;
}

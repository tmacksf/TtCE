//
// Created by Thomas Mack on 23/12/2022.
//

#include "game.h"
#include "defsEnums.h"

using namespace std;

void initializeMoveGenerationInformation() {
  Bitboard::initAttackTables();
  Magics::generateSlidingAttackTables();
}

int Game() {
  //int status = gameLoop(STARTING_FEN, BLACK);
  gameState gs{};
  gs.initialise("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
  vector<Move> moves;
  moveGen::legalMoves(gs,moves);
  for (Move m: moves){
      m.printMove();
  }
  cout << moves.size() << "\n\n";

  for (int i = 0; i < 64; i++) {

  }

  return 0;
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
    if (moves.at(i).fromSquare == moveLocation[0] &&
        moves.at(i).toSquare == moveLocation[1] &&
        moves.at(i).promotedPiece == moveLocation[2]) {
      return moves.at(i);
    }
  }
  return Move{-1};
}

void playerTurn(int *moveLocation) {
  string move = "";

  cin >> move;
  parseUCIMove(move, moveLocation);
  cout << moveLocation[0] << moveLocation[1] << moveLocation[2] << endl;
}

Move engineTurn(const gameState &gs) {
  std::vector<Move> moveVec;

  moveGen::legalMoves(gs, moveVec);
  int moveCount = moveVec.size();
  int move = rand() % moveCount;
  return moveVec[move];
}

int gameLoop(const string &startFen, Color engineColor) {
  Color playerColor = ~engineColor;
  gameState gs{}; // current board state
  gs.initialise(startFen);
  bool hasWon = false;
  while (!hasWon) {
    // Print current game state
    gs.printing();
    bool validMove = false;
    std::vector<Move> legalMoves;
    moveGen::legalMoves(gs, legalMoves);
    // loop to make sure move is legal

    if (gs.getTurn() == engineColor) {
      gs.makeMove(engineTurn(gs));
    } else {
      while (!validMove) {
        int moveLocation[] = {0, 0, 0};
        playerTurn(moveLocation);
        Move m = isMoveLegal(legalMoves, moveLocation);
        cout << m.fromSquare << m.toSquare << endl;
        if (m.fromSquare != -1) {
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
    hasWon = gs.isMoveCheckmate();
  }
  return 0;
}

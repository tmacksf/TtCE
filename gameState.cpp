//
// Created by Thomas Mack on 28/12/2022.
//

#include "gameState.h"
#include "defsEnums.h"

gameState::gameState() {
  m_turn = WHITE;
  for (Bitboard &m_bitboard : m_bitboards) {
    m_bitboard = Bitboard();
  }
  m_attacking = BLACK;

  for (int i = 0; i < 4; i++) {
    m_castling[i] = true;
  }
}

void gameState::initialise(std::string fen) {
  // splitting the string into two parts, first to define the board position and
  // second to define castling etc.
  int i;

  std::string s;
  std::stringstream ss(fen);
  std::vector<std::string> fenSeperated;
  while (std::getline(ss, s, ' ')) {
    fenSeperated.push_back(s);
  }

  // board position section
  int boardPos = 63;
  for (i = 0; i < fenSeperated[0].length(); i++) {
    if (std::isdigit(fenSeperated[0][i])) {
      // not sure if I should be casting like this
      boardPos -= int(fenSeperated[0][i]) - 48;
    } else if (fen[i] == '/') {
      ;
    } else {
      char temp = fenSeperated[0][i];
      m_bitboards[charToPiece[temp]].setBitAt(boardPos);
      boardPos -= 1;
    }
  }

  // turn section
  if (fenSeperated[1][0] == 'w') {
    this->m_turn = WHITE;
    this->m_attacking = BLACK;
  } else {
    this->m_turn = BLACK;
    this->m_attacking = WHITE;
  }

  // castling section
  m_castling[0] = false;
  m_castling[1] = false;
  m_castling[2] = false;
  m_castling[3] = false;
  for (i = 0; i < fenSeperated[2].length(); i++) {
    if (fenSeperated[2][i] == 'K')
      m_castling[0] = true;
    if (fenSeperated[2][i] == 'Q')
      m_castling[1] = true;
    if (fenSeperated[2][i] == 'k')
      m_castling[2] = true;
    if (fenSeperated[2][i] == 'q')
      m_castling[3] = true;
  }

  int enPSquare = -1;
  char tempEP[2];
  for (i = 0; i < fenSeperated[3].length(); i++) {
    // Todo make en passant square work...
    tempEP[i] = fenSeperated[3][i];
  }
  m_enPassantSquare = enPSquare;

  // TODO half move counter and full move counter
}

void gameState::printing() const {
  std::string outString;
  int counter = 0;
  int square = 64;
  for (int row = 0; row < 8; row++) {
    outString = std::to_string(8 - row);
    outString += "      ";
    for (int col = 0; col < 8; col++) {
      square--;
      int flag = 0;
      for (int j = 0; j < 12; j++) {
        if (m_bitboards[j].getBitAt(square) == 1) {
          // outString.append(1, pieceToChar[j]);
          outString += pieceToSymbolDark[j];
          outString += "  ";
          flag = 1;
        }
      }
      if (!flag) {
        outString.append(1, '-');
        outString += "  ";
      }
      counter += 1;
      if (counter == 8) {
        std::cout << outString << std::endl;
        counter = 0;
        outString = "";
      }
    }
  }

  std::cout << "\n       A  B  C  D  E  F  G  H" << std::endl;
  std::cout << "Turn: " << m_turn << " Attacking: " << m_attacking << std::endl;
}

// this needs to be sped up. Can do that with method template so each type of
// move is executed
void gameState::makeMove(const Move &move) {
  // done on a copy of the parent game state to create a child game state based
  // on a move
  m_bitboards[move.piece].unSetBitAt(move.fromSquare);
  m_bitboards[move.piece].setBitAt(move.toSquare);

  if (move.captureFlag) {
    removePieceOnCapture(move.toSquare);
  }

  // white castling priv
  if (move.piece == ROOK) {
    if (move.fromSquare == h1)
      m_castling[0] = false;
    else if (move.fromSquare == a1)
      m_castling[1] = false;
  }
  // black castling priv
  if (move.piece == ROOK + 6) {
    if (move.fromSquare == h8)
      m_castling[2] = false;
    else if (move.fromSquare == a8)
      m_castling[3] = false;
  }

  if (move.piece == K)
    m_castling[0] = false, m_castling[1] = false;
  if (move.piece == k)
    m_castling[2] = false, m_castling[3] = false;
  if (move.castleFlag) {
    // if it is black's turn
    if (m_turn) {
      // short castle
      if (move.toSquare == g8) {
        m_bitboards[ROOK + 6].unSetBitAt(h8);
        m_bitboards[ROOK + 6].setBitAt(f8);
      } else {
        m_bitboards[ROOK + 6].unSetBitAt(a8);
        m_bitboards[ROOK + 6].setBitAt(d8);
      }
      m_castling[2] = false;
      m_castling[3] = false;
    } else {
      // short castle
      if (move.toSquare == g1) {
        m_bitboards[ROOK].unSetBitAt(h1);
        m_bitboards[ROOK].setBitAt(f1);
      } else {
        m_bitboards[ROOK].unSetBitAt(a1);
        m_bitboards[ROOK].setBitAt(d1);
      }
      m_castling[0] = false;
      m_castling[1] = false;
    }
  }

  // just going to set en passant to the default unless a pawn is double pushed
  m_enPassantSquare = -1;
  if (move.doublePushFlag) {
    // will be -8 if it is white and +8 if it is black
    m_enPassantSquare = move.toSquare - 8 + (16 * m_turn);
  }

  if (move.enPassantFlag) {
    // if it is white's turn it will be -8 on the move to square whereas if it
    // is black it will be a plus 8 thus 16 * 1 + 8
    removePieceOnCapture(move.toSquare - 8 + (16 * m_turn));
  }

  if (move.promotedPiece) {
    m_bitboards[move.piece].unSetBitAt(move.toSquare);
    m_bitboards[move.promotedPiece].setBitAt(move.toSquare);
  }

  m_turn = ~m_turn;
  m_attacking = ~m_attacking;
}

// TODO clean up this function. It's gross
void gameState::stateToFen() {
  std::string output;
  BB allPcs = allPieces();
  int empty_squares = 0;
  int col_counter = 0; // for counting the number of columns
  int num_slashes = 0; // another dumb way of doing this but whatever
  BB location;
  for (int i = 63; i > -1; i--) {
    location = 1ULL << i;
    if (location & allPcs) {
      if (empty_squares)
        output += std::to_string(empty_squares);
      output += pieceToChar[getPieceAt(i)];
      empty_squares = 0;
      col_counter += 1;
    } else {
      empty_squares += 1;
      col_counter += 1;
    }
    if (col_counter == 8) {
      if (empty_squares)
        output += std::to_string(empty_squares);
      empty_squares = 0;
      col_counter = 0;
      num_slashes += 1;
      if (num_slashes < 8)
        output += '/';
    }
  }

  output += ' ';
  m_turn ? output += 'b' : output += 'w';
  output += ' ';

  // todo deal with no castling
  if (m_castling[0])
    output += 'K';
  if (m_castling[1])
    output += 'Q';
  if (m_castling[2])
    output += 'k';
  if (m_castling[3])
    output += 'q';
  output += ' ';
  (m_enPassantSquare != -1) ? output += boardMap[m_enPassantSquare]
                            : output += '-';
  output += " 0 1";

  std::cout << output << std::endl;
};

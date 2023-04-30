//
// Created by Thomas Mack on 28/12/2022.
//

#include "gameState.hpp"
#include "defsEnums.hpp"

// initialise transposition table
gameState::TT gameState::hashTable[transpositionTableSize];

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

void gameState::clear() {
  for (Bitboard &b : m_bitboards) {
    b = Bitboard();
  }
  m_turn = WHITE;
  m_attacking = BLACK;
  for (int i = 0; i < 4; i++) {
    m_castling[i] = false;
  }
  int m_enPassantSquare = -1;
  int m_ply = 0;
  int m_fullMoveCounter = 0;
  int halfMoveCounter = 0;
  BB m_attackers = 0ULL;
}

void gameState::initialise(std::string fen) {
  // splitting the string into two parts, first to define the board position and
  // second to define castling etc.
  int i;
  m_ply = 0;

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
    tempEP[i] = fenSeperated[3][i];
  }
  /*
  if (fenSeperated[3][i] != '-') {
    m_enPassantSquare = charToFile[fenSeperated[3][0]] +
                        8 * ((int)fenSeperated[3][1] - '0') - 8;

  } else
    m_enPassantSquare = enPSquare;
  */
  m_enPassantSquare = enPSquare;

  // TODO half move counter and full move counter
}

void gameState::printing() const {
  std::string outString;
  int counter = 0;
  int square = 64;
  for (int row = 0; row < 8; row++) {
    outString = std::to_string(8 - row);
    outString += "    ";
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

  std::cout << "\n     A  B  C  D  E  F  G  H" << std::endl;
  std::string attacking = "Black";
  std::string turn = "White";
  if (m_turn)
    attacking = "White", turn = "Black";

  std::cout << "     Turn: " << turn << " Attacking: " << attacking
            << std::endl;
  char cWK = m_castling[0] ? 'K' : '-';
  char cWQ = m_castling[1] ? 'Q' : '-';
  char cBK = m_castling[2] ? 'k' : '-';
  char cBQ = m_castling[3] ? 'q' : '-';
  std::cout << "     Castling: " << cWK << cWQ << cBK << cBQ;

  std::string epSquarePrinting =
      m_enPassantSquare == -1 ? "--" : boardMap[m_enPassantSquare];

  std::cout << "  EP:" << epSquarePrinting << std::endl;
}

// Method to make a given move, returns zobrist hash of new move
BB gameState::makeMove(const Move &move, BB moveHash) {
  m_ply++;
  if (m_turn) // if black moves increment the full move counter
    m_fullMoveCounter++;

  // done on a copy of the parent game state to create a child game state based
  // on a move
  m_bitboards[move.m_piece].unSetBitAt(move.m_fromSquare);
  m_bitboards[move.m_piece].setBitAt(move.m_toSquare);

  moveHash ^= zArray[move.m_piece][move.m_fromSquare];
  moveHash ^= zArray[move.m_piece][move.m_toSquare];

  // don't need to do this before moving piece as we only look at opposing side
  if (move.m_captureFlag) {
    int captured = removePieceOnCapture(move.m_toSquare);
    moveHash ^= zArray[captured][move.m_toSquare];
  }

  // white castling priv
  if (move.m_piece == R) {
    if (move.m_fromSquare == h1 && m_castling[0]) {
      m_castling[0] = false;
      moveHash ^= zCastle[0];
    } else if (move.m_fromSquare == a1 && m_castling[1]) {
      m_castling[1] = false;
      moveHash ^= zCastle[1];
    }
  }
  // black castling priv
  if (move.m_piece == r) {
    if (move.m_fromSquare == h8 && m_castling[2]) {
      m_castling[2] = false;
      moveHash ^= zCastle[2];
    } else if (move.m_fromSquare == a8 && m_castling[3]) {
      m_castling[3] = false;
      moveHash ^= zCastle[3];
    }
  }

  if (move.m_piece == K) {
    if (m_castling[0]) {
      moveHash ^= zCastle[0];
    }
    if (m_castling[1]) {
      moveHash ^= zCastle[1];
    }
    m_castling[0] = false, m_castling[1] = false;
  }
  if (move.m_piece == k) {
    if (m_castling[2])
      moveHash ^= zCastle[2];
    if (m_castling[3])
      moveHash ^= zCastle[3];
    m_castling[2] = false, m_castling[3] = false;
  }

  if (move.m_castleFlag) {
    // if it is black's turn
    if (m_turn) {
      // short castle
      if (move.m_toSquare == g8) {
        m_bitboards[r].unSetBitAt(h8);
        m_bitboards[r].setBitAt(f8);
        moveHash ^= zArray[r][h8] ^ zArray[r][f8];
      } else {
        m_bitboards[r].unSetBitAt(a8);
        m_bitboards[r].setBitAt(d8);
        moveHash ^= zArray[r][a8] ^ zArray[r][d8];
      }
      m_castling[2] = false;
      m_castling[3] = false;
    } else {
      // short castle
      if (move.m_toSquare == g1) {
        m_bitboards[ROOK].unSetBitAt(h1);
        m_bitboards[ROOK].setBitAt(f1);
        moveHash ^= zArray[R][h1] ^ zArray[R][f1];
      } else {
        m_bitboards[ROOK].unSetBitAt(a1);
        m_bitboards[ROOK].setBitAt(d1);
        moveHash ^= zArray[R][a1] ^ zArray[R][d1];
      }
      m_castling[0] = false;
      m_castling[1] = false;
    }
  }

  // This takes care of putting the en passant square back to normal
  if (m_enPassantSquare != -1)
    moveHash ^= zEP[m_enPassantSquare % 8];

  // just going to set en passant to the default unless a pawn is double pushed
  m_enPassantSquare = -1;
  if (move.m_doublePushFlag) {
    // will be -8 if it is white and +8 if it is black
    m_enPassantSquare = move.m_toSquare - 8 + (16 * m_turn);
    moveHash ^= zEP[m_enPassantSquare % 8];
  }

  if (move.m_enPassantFlag) {
    // if it is white's turn it will be -8 on the move to square whereas if it
    // is black it will be a plus 8 thus 16 * 1 + 8
    if (m_turn) {
      m_bitboards[P].unSetBitAt(move.m_toSquare + 8);
      moveHash ^= zArray[P][move.m_toSquare + 8];
    } else {
      m_bitboards[p].unSetBitAt(move.m_toSquare - 8);
      moveHash ^= zArray[p][move.m_toSquare - 8];
    }
    // removePieceOnCapture(move.m_toSquare - 8 + (16 * m_turn));
  }

  if (move.m_promotedPiece) {
    // need to unset the bit we set earlier
    m_bitboards[move.m_piece].unSetBitAt(move.m_toSquare);
    m_bitboards[move.m_promotedPiece].setBitAt(move.m_toSquare);

    moveHash ^= zArray[move.m_piece][move.m_toSquare];
    moveHash ^= zArray[move.m_promotedPiece][move.m_toSquare];
  }

  m_turn = ~m_turn;
  m_attacking = ~m_attacking;
  moveHash ^= zBlackToMove;

  return moveHash;
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

// Method to get the piece (and ray if applicable) that is currently checking
// the king
// Quick way to optimise would be to place pieces that check the most frequently
// before
/*
void gameState::findAttackingLocation() {
  m_attackers = 0ULL;
  int kingSquare = m_bitboards[6 * m_turn].getLeastSignificantBit();
  int attackerIndex;

  BB blockers = allPieces();
  BB squareAttacks = Magics::getRookAttacks(kingSquare, blockers) &
                     (getPieceBitboard(ROOK, m_attacking) |
                      getPieceBitboard(QUEEN, m_attacking));

  BB diagonalAttacks = Magics::getBishopAttacks(kingSquare, blockers) &
                       (getPieceBitboard(QUEEN, m_attacking) |
                        getPieceBitboard(BISHOP, m_attacking));

  BB pawnAttacks = Bitboard::pawnAttacks[m_turn][kingSquare] &
                   getPieceBitboard(PAWN, m_attacking);
  BB knightAttacks =
      Bitboard::knightMoves[kingSquare] & getPieceBitboard(KNIGHT, m_attacking);
  int dist;
  if (squareAttacks) {
    // king has to move or has to be blocked/captured
    attackerIndex = Bitboard::getLeastSignificantBit(squareAttacks);
    dist = kingSquare - attackerIndex;
    Direction attackDirection;

    // get direction of attacker
    if (dist < 8 && dist > 0) {
      attackDirection = EAST;
    } else if (0 > dist && dist > -8) {
      attackDirection = WEST;
      dist = -1 * dist; // (flip first bit)
    } else if (dist > 7) {
      attackDirection = SOUTH;
      dist = dist / 8;
    } else {
      attackDirection = NORTH;
      dist = dist / 8;
      dist = -1 * dist;
    }
    m_attackers = Bitboard::rayAttack(attackDirection, kingSquare, dist);

  } else if (diagonalAttacks) {
    // king has to move or has to be blocked/captured
    attackerIndex = Bitboard::getLeastSignificantBit(diagonalAttacks);
    dist = attackerIndex - kingSquare;
    Direction attackDirection;

    // get direction
    if (dist < 0 && dist % 7 == 0) {
      attackDirection = SOUTH_WEST;
      dist = -1 * dist / 7;
    } else if (dist < 0 && dist % 9 == 0) {
      attackDirection = SOUTH_EAST;
      dist = -1 * dist / 9;
    } else if (dist > 0 && dist % 7 == 0) {
      attackDirection = NORTH_EAST;
      dist = dist / 7;
    } else {
      attackDirection = NORTH_WEST;
      dist = dist / 9;
    }

    // TODO: Make sure its completely correct
    m_attackers = Bitboard::rayAttack(attackDirection, kingSquare, dist);

  } else if (knightAttacks) {
    // has to be captured or king has to move
    m_attackers = knightAttacks;
  } else if (pawnAttacks) {
    // has to be captured or king has to move
    m_attackers = pawnAttacks;
  }
}
*/

BB gameState::zArray[12][64];
BB gameState::zEP[8];
BB gameState::zCastle[4];
BB gameState::zBlackToMove;

void gameState::initZobrist() {
  BB rSeed = 3671358418; // for reproducing results
  // std::random_device rd;
  // std::mt19937_64 eng(rd());
  std::mt19937_64 eng(rSeed);
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
  zBlackToMove = distr(eng);
}

BB gameState::getHash() const {
  BB zHash = 0ULL;
  int square;
  int pt;
  // piece hashing
  for (int i = 0; i < 12; i++) {
    BB pieces = m_bitboards[i].getValue();
    while (pieces) {
      square = pop_lsb(pieces);
      zHash ^= zArray[i][square];
    }
  }
  // EP hashing by column (Get double push square)
  if (m_enPassantSquare != -1)
    zHash ^= zEP[m_enPassantSquare % 8];

  // castling
  for (int i = 0; i < 4; i++) {
    if (m_castling[i])
      zHash ^= zCastle[i];
  }

  if (m_turn)
    zHash ^= zBlackToMove;

  return zHash;
}

void gameState::clearTable() {
  for (int i = 0; i < transpositionTableSize; i++) {
    gameState::hashTable[i].zkey = 0;
    gameState::hashTable[i].depth = 0;
    gameState::hashTable[i].flag = 0;
    gameState::hashTable[i].score = 0;
  }
}

void gameState::allInfo() {
  stateToFen();
  std::cout << "Attacking: " << m_attacking << std::endl;
}

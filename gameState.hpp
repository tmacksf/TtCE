//
// Created by Thomas Mack on 28/12/2022.
//

#ifndef TTCE_GAMESTATE_HPP
#define TTCE_GAMESTATE_HPP

#include "Move.hpp"

namespace TtCE {

class gameState {
private:
  Bitboard m_bitboards[12]; // order: K, Q, R, B, N, P, k, q, r, b, n, p
  Color m_turn;             // color based on WHITE = 0, BLACK = 1, NONE = 2
  Color m_attacking;
  bool m_castling[4];         // order: K, Q, k, q
  int m_enPassantSquare = -1; // index of the en passant square
  int m_ply = 0;              // Used in search as a distance from root node
  int m_fullMoveCounter = 0;
  int halfMoveCounter = 0;

  // Information about attackers
  // BB m_attackers = 0ULL; // location of attacking piece and ray its producing

public:
  // zobrist information
  static BB zArray[12][64];
  static BB zEP[8];
  static BB zCastle[4];
  static BB zBlackToMove;

  typedef struct {
    BB zkey;
    int depth;
    int flag;
    int score;
  } TT;

  static TT hashTable[transpositionTableSize];

public:
  gameState();
  // getter methods
  inline Bitboard &getBitboardMut(int index) { return m_bitboards[index]; }
  [[nodiscard]] inline Bitboard getBitboard(int index) const {
    return m_bitboards[index];
  }
  [[nodiscard]] inline bool getCastling(int index) const {
    return m_castling[index];
  }
  [[nodiscard]] inline Color getTurn() const { return m_turn; }
  [[nodiscard]] inline Color getAttacking() const { return m_attacking; }
  [[nodiscard]] inline int getEPSquare() const { return m_enPassantSquare; }
  //[[nodiscard]] inline BB getAttackingPieces() const { return m_attackers; }
  [[nodiscard]] inline int getPly() const { return m_ply; }
  inline void setPly(int ply) { m_ply = ply; }
  void allInfo();

  // member functions
  void printing() const;
  void stateToFen();
  void initialise(std::string fen);
  bool isMoveCheckmate();
  BB makeMove(const Move &move, BB moveHash);
  void findAttackingLocation();
  void clear();

  int inline getPieceAt(int square) const {
    int piece = 0;
    for (Bitboard bitboard : m_bitboards) {
      if (bitboard.getValue() & 1ULL << square)
        return piece;
      piece += 1;
    }
    throw std::runtime_error("No piece at position");
  }

  // TODO: Optimise
  int inline getEnemyPieceAt(int square) const {
    BB location = 1ULL << square;
    for (int i = 6 * m_attacking; i < k + 6 * m_attacking; i++) {
      if (m_bitboards[i].getValue() & location)
        return i;
    }
    if (m_enPassantSquare != -1)
      return P;
    return -1;
  }

  // TODO: Change to
  int inline getPieceCount(int piece) const {
    return m_bitboards[piece].bitCount();
  }

  [[nodiscard]] BB inline friendlyBoard() const {
    BB friendly = 0ULL;
    for (int i = 0; i < 6; i++)
      friendly |= m_bitboards[i + 6 * m_turn].getValue();
    return friendly;
  }

  [[nodiscard]] BB inline enemyBoard() const {
    BB enemy = 0ULL;
    for (int i = 0; i < 6; i++)
      enemy |= m_bitboards[i + 6 * m_attacking].getValue();
    return enemy;
  }

  [[nodiscard]] BB inline allPieces() const {
    BB all = 0ULL;
    for (Bitboard bitboard : m_bitboards)
      all |= bitboard.getValue();
    return all;
  }

  // TODO: Refactor and make function with the same name that does not return
  // the value but the object reference
  [[nodiscard]] BB inline getPieceBitboard(PieceTypes pt, Color c) const {
    return m_bitboards[pt + 6 * c].getValue();
  }

  static int inline getPiece(PieceTypes pt, Color c) { return pt + 6 * c; }

  int inline removePieceOnCapture(int square) {
    // only have to do half of the list as it is opposition
    BB position = 1ULL << square;
    for (int i = 0 + (6 * m_attacking); i < 6 + (6 * m_attacking); i++) {
      if (position & m_bitboards[i].getValue()) {
        m_bitboards[i].unSetBitAt(square);
        return i;
      }
    }
    // TODO: Make sure to be cautious of this as it may try to index zobrist
    // array wrong
    return -1; // worst case
  }

  // Method to see if a square is attacked, take position and attacking color
  // TODO: Rewrite function with updated getting functions
  [[nodiscard]] int inline attacked(int position, Color attackingColor) const {
    BB allBlockers = allPieces();
    Color friendlyColor = ~attackingColor;

    int attackers = 0;
    // leaping pieces
    if (Bitboard::pawnAttacks[friendlyColor][position] &
        m_bitboards[PAWN + 6 * attackingColor].getValue())
      attackers += 1;
    if (Bitboard::knightMoves[position] &
        m_bitboards[KNIGHT + 6 * attackingColor].getValue())
      attackers += 1;
    if (Bitboard::kingMoves[position] &
        m_bitboards[KING + 6 * attackingColor].getValue())
      attackers += 1;

    // sliding pieces
    BB squareAttackers = m_bitboards[QUEEN + attackingColor * 6].getValue() |
                         m_bitboards[ROOK + attackingColor * 6].getValue();
    BB square = Magics::getRookAttacks(position, allBlockers);
    if (square & squareAttackers)
      attackers += 1;

    BB diagonalAttackers = m_bitboards[BISHOP + attackingColor * 6].getValue() |
                           m_bitboards[QUEEN + attackingColor * 6].getValue();
    BB diagonals = Magics::getBishopAttacks(position, allBlockers);
    if (diagonals & diagonalAttackers)
      attackers += 1;

    return attackers;
  }

  [[nodiscard]] int inline attacked(int position, Color attackingColor,
                                    BB customBlockers) const {
    Color friendlyColor = ~attackingColor;

    int attackers = 0;
    // leaping pieces
    if (Bitboard::pawnAttacks[friendlyColor][position] &
        m_bitboards[PAWN + 6 * attackingColor].getValue())
      attackers += 1;
    if (Bitboard::knightMoves[position] &
        m_bitboards[KNIGHT + 6 * attackingColor].getValue())
      attackers += 1;
    if (Bitboard::kingMoves[position] &
        m_bitboards[KING + 6 * attackingColor].getValue())
      attackers += 1;

    // sliding pieces
    BB squareAttackers = m_bitboards[QUEEN + attackingColor * 6].getValue() |
                         m_bitboards[ROOK + attackingColor * 6].getValue();
    BB square = Magics::getRookAttacks(position, customBlockers);
    if (square & squareAttackers)
      attackers += 1;

    BB diagonalAttackers = m_bitboards[BISHOP + attackingColor * 6].getValue() |
                           m_bitboards[QUEEN + attackingColor * 6].getValue();
    BB diagonals = Magics::getBishopAttacks(position, customBlockers);
    if (diagonals & diagonalAttackers)
      attackers += 1;

    return attackers;
  }

  [[nodiscard]] int inline isKingInCheck(Color attackingColor) const {
    return attacked(
        Bitboard::getLeastSignificantBit(
            m_bitboards[KING + (1 - attackingColor) * 6].getValue()),
        attackingColor);
  }
  [[nodiscard]] int inline isKingInCheck() const {
    Color attackingColor = m_attacking;
    return attacked(
        Bitboard::getLeastSignificantBit(
            m_bitboards[KING + (1 - attackingColor) * 6].getValue()),
        attackingColor);
  }

  // Null move
  BB inline makeNullMove(BB hash) {
    m_ply++;
    m_turn = ~m_turn;
    m_attacking = ~m_attacking;
    hash ^= zBlackToMove;

    if (m_enPassantSquare != -1)
      hash ^= zEP[m_enPassantSquare % 8];
    m_enPassantSquare = -1;

    return hash;
  }

  // Zobrist hashing
  static void initZobrist();
  BB getHash() const;
  // hash table
  static void clearTable();
  static void hashTableEntry(TT t);

  // checks to see if the hash entry is exact. If it is returns true
  static bool inline checkHashEntry(BB zKey) {
    TT *hashEntry = &hashTable[zKey % transpositionTableSize];

    if (hashEntry->flag == hashFlagExact)
      return true;
    else
      return false;
  }

  static inline TT *getHashForPV(BB zKey) {
    return &hashTable[zKey % transpositionTableSize];
  }

  static inline int getHashEntry(BB zKey, int alpha, int beta, int depth) {
    TT *hashEntry = &hashTable[zKey % transpositionTableSize];
    if (hashEntry->zkey == zKey) {
      if (hashEntry->depth >= depth) {
        int score = hashEntry->score;
        if (hashEntry->flag == hashFlagExact) {
          return score;
        }
        if ((hashEntry->flag == hashFlagAlpha) && (score <= alpha)) {
          return alpha; // alpha cutoff
        }
        if ((hashEntry->flag == hashFlagBeta) && (score >= beta)) {
          return beta; // beta cutoff
        }
      }
    }
    return valUnknown;
  }

  static void inline insertHash(BB zKey, int score, int depth, int flag,
                                int ply) {
    TT *hashLoc = &hashTable[zKey % transpositionTableSize];

    if (score > MATE_SCORE)
      score += ply;
    if (score < MATE_SCORE)
      score -= ply;

    hashLoc->zkey = zKey;
    hashLoc->score = score;
    hashLoc->depth = depth;
    hashLoc->flag = flag;
  }
};
} // namespace TtCE

#endif // TTCE_GAMESTATE_HPP

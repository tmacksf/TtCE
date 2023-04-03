//
// Created by Thomas Mack on 28/12/2022.
//

#ifndef CHESS_CPP_GAMESTATE_H
#define CHESS_CPP_GAMESTATE_H

#include "Move.h"
#include "defsEnums.h"
#include <stdexcept>

class gameState {
private:
  Bitboard m_bitboards[12]; // order: K, Q, R, B, N, P, k, q, r, b, n, p
  Color m_turn;             // color based on WHITE = 0, BLACK = 1, NONE = 2
  Color m_attacking;
  bool m_castling[4];         // order: K, Q, k, q
  int m_enPassantSquare = -1; // index of the en passant square
  int m_ply = 0;
  int m_fullMoveCounter = 0;
  int halfMoveCounter = 0;

  // Information about attackers
  BB m_attackers = 0ULL; // location of attacking piece and ray its producing

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
  [[nodiscard]] inline BB getAttackingPieces() const { return m_attackers; }
  [[nodiscard]] inline int getPly() const { return m_ply; }

  // member functions
  void printing() const;
  void stateToFen();
  void initialise(std::string fen);
  bool isMoveCheckmate();
  void makeMove(const Move &move);
  void findAttackingLocation();

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

  void inline removePieceOnCapture(int square) {
    // only have to do half of the list as it is opposition
    // TODO: add 1 to this so the king isn't looked at too (too domed rn)
    BB position = 1ULL << square;
    for (int i = 0 + (6 * m_attacking); i < 6 + (6 * m_attacking); i++) {
      if (position & m_bitboards[i].getValue()) {
        m_bitboards[i].unSetBitAt(square);
        break;
      }
    }
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

  [[nodiscard]] BB inline isKingInCheck(Color attackingColor) const {
    return attacked(
        Bitboard::getLeastSignificantBit(
            m_bitboards[KING + (1 - attackingColor) * 6].getValue()),
        attackingColor);
  }
};

#endif // CHESS_CPP_GAMESTATE_H

//
// Created by Thomas Mack on 28/12/2022.
//

#ifndef CHESS_CPP_GAMESTATE_H
#define CHESS_CPP_GAMESTATE_H

#include "Move.h"

class gameState {
private:
  Bitboard m_bitboards[12]; // order: K, Q, R, B, N, P, k, q, r, b, n, p
  Color m_turn;             // color based on WHITE = 0, BLACK = 1, NONE = 2
  Color m_attacking;
  bool m_castling[4]{}; // order: white king side, white queen side, black king
                        // side, white king side
  int m_enPassantSquare = -1; // index of the en passant square activated when a
                              // pawn is double pushed
  // TODO add a bitboard offset variable for accessing friendly and enemy
  // m_bitboards

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

  // member functions
  void printing() const;
  void stateToFen();
  void initialise(std::string fen);
  bool isMoveCheckmate();

  int inline getPieceAt(int square) {
    int piece = 0;
    for (Bitboard bitboard : m_bitboards) {
      if (bitboard.getValue() & 1ULL << square)
        return piece;
      piece += 1;
    }
    throw std::runtime_error("No piece at position");
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

  [[nodiscard]] BB inline getPieceBitboard(PieceTypes pt, Color c) const {
    return m_bitboards[pt + 6 * c].getValue();
  }

  static int inline getPiece(PieceTypes pt, Color c) { return pt + 6 * c; }

  // Move making functions
  void makeMove(const Move &move);

  void inline removePieceOnCapture(int square) {
    // only have to do half of the list as it is opposition
    BB position = 1ULL << square;
    for (int i = 0 + (6 * m_attacking); i < 6 + (6 * m_attacking); i++) {
      if (position & m_bitboards[i].getValue()) {
        m_bitboards[i].unSetBitAt(square);
        break;
      }
    }
  }

  void unMakeMove(Move *moves[]);
  void unMakeMove(Move &move);

  // TODO Clean up and optimize
  // Method to see if a square is attacked, take position and attacking color
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

  /*[[nodiscard]] BB inline isKingInCheck() const {
    return attacked(m_bitboards[KING +
  friendlyColor*6].getLeastSignificantBit(), attackingColor); // TODO make sure
  this change works as intended (change was getting lsb of m_bitboards)
  }
  */

  [[nodiscard]] BB inline isKingInCheck(Color attackingColor) const {
    return attacked(
        Bitboard::getLeastSignificantBit(
            m_bitboards[KING + (1 - attackingColor) * 6].getValue()),
        attackingColor);
  }
};

#endif // CHESS_CPP_GAMESTATE_H

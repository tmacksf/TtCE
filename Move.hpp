//
// Created by Thomas Mack on 05/01/2023.
//

#ifndef CHESS_CPP_MOVE_H
#define CHESS_CPP_MOVE_H
#include "Magics.hpp"
#include "defsEnums.hpp"

class Move {
  // TODO: Make private
public:
  int m_fromSquare;
  int m_toSquare;
  int m_piece;
  bool m_captureFlag;
  bool m_doublePushFlag;
  int m_promotedPiece; // going to have this as an int which specifies the type
                       // of piece being promoted to
  bool m_castleFlag;
  bool m_enPassantFlag;
  int m_moveScore;

public:
  // mvv_lva
  static constexpr int mvv_lva[12][12] = {
      // White pieces
      {600, 500, 400, 300, 200, 100, 600, 500, 400, 300, 200, 100},
      {601, 501, 401, 301, 201, 101, 601, 501, 401, 301, 201, 101},
      {602, 502, 402, 202, 202, 102, 602, 502, 402, 302, 202, 102},
      {603, 503, 403, 303, 203, 103, 603, 503, 403, 303, 203, 103},
      {604, 504, 404, 304, 204, 104, 604, 504, 404, 304, 204, 104},
      {605, 505, 405, 305, 205, 105, 605, 505, 405, 305, 205, 105},
      // Black pieces
      {600, 500, 400, 300, 200, 100, 600, 500, 400, 300, 200, 100},
      {601, 501, 401, 301, 201, 101, 601, 501, 401, 301, 201, 101},
      {602, 502, 402, 202, 202, 102, 602, 502, 402, 302, 202, 102},
      {603, 503, 403, 303, 203, 103, 603, 503, 403, 303, 203, 103},
      {604, 504, 404, 304, 204, 104, 604, 504, 404, 304, 204, 104},
      {605, 505, 405, 305, 205, 105, 605, 505, 405, 305, 205, 105},
  };

  // [killer move number][ply]
  static Move killerMove[2][MAX_DEPTH];
  // [piece][toSquare]
  static int historyMove[12][64];

  Move(int fromSquare, int toSquare, int piece, bool castleFlag,
       bool enPassantFlag, bool captureFlag, bool doublePushFlag,
       int promotedPiece)
      : m_fromSquare(fromSquare), m_toSquare(toSquare), m_piece(piece),
        m_castleFlag(castleFlag), m_enPassantFlag(enPassantFlag),
        m_captureFlag(captureFlag), m_doublePushFlag(doublePushFlag),
        m_promotedPiece(promotedPiece) {
    // m_moveScore = m_piece;
    m_moveScore = 0;
  }
  Move() = default;

  // Move(int fromSquare) : m_fromSquare(fromSquare) {}
  Move(int fromSquare, int moveScore)
      : m_fromSquare(fromSquare), m_moveScore(moveScore) {}

  bool operator<(const Move &mRight) {
    return m_moveScore < mRight.m_moveScore;
  }
  // TODO: Figure out if i need to add more to this
  bool operator==(const Move &mRight) {
    // don't need to add castleFlag and doublePushFlag as they are implied when
    // a king or pawn move more than one square
    return m_fromSquare == mRight.m_fromSquare &&
           m_toSquare == mRight.m_toSquare && m_piece == mRight.m_piece &&
           m_promotedPiece == mRight.m_promotedPiece &&
           m_enPassantFlag == mRight.m_enPassantFlag &&
           m_captureFlag == mRight.m_captureFlag;
  }

  void printMove(int debug = 0) const {
    if (debug) {
      std::cout << "Piece: " << pieceToChar[m_piece]
                << " From: " << boardMap[m_fromSquare] << boardMap[m_toSquare];
      if (m_promotedPiece)
        std::cout << pieceToChar[m_promotedPiece];
      if (m_captureFlag)
        std::cout << " Capture";
      if (m_doublePushFlag)
        std::cout << " Pawn double push";
      if (m_castleFlag)
        std::cout << " Castle";
      if (m_enPassantFlag)
        std::cout << " En passant";
      std::cout << "\n";
    } else {
      std::cout << boardMap[m_fromSquare] << boardMap[m_toSquare];
      if (m_promotedPiece)
        std::cout << pieceToChar[m_promotedPiece];
    }
  }

  // will be inline for now
  void inline scoreMove(int capturedPiece, int ply) {
    if (m_captureFlag) {
      m_moveScore += mvv_lva[m_piece][capturedPiece] + 10000;
    } else {
      if (killerMove[0][ply] == *this) {
        m_moveScore += 9000;
      }
      if (killerMove[1][ply] == *this)
        m_moveScore += 8000;
      else
        m_moveScore += historyMove[m_piece][m_toSquare];
    }
  }

  // set a high score so it will be sorted to first and be the first move
  void inline pvMove() { m_moveScore += 20000; }
};

#endif // CHESS_CPP_MOVE_H

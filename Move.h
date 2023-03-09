//
// Created by Thomas Mack on 05/01/2023.
//

#ifndef CHESS_CPP_MOVE_H
#define CHESS_CPP_MOVE_H
#include "Magics.h"

class Move {
public:
  int fromSquare;
  int toSquare;
  int piece;
  bool captureFlag;
  bool doublePushFlag;
  int promotedPiece; // going to have this as an int which specifies the type of
                     // piece being promoted to
  bool castleFlag;
  bool enPassantFlag;

  Move(int fromSquare, int toSquare, int piece, bool castleFlag,
       bool enPassantFlag, bool captureFlag, bool doublePushFlag,
       int promotedPiece)
      : fromSquare(fromSquare), toSquare(toSquare), piece(piece),
        castleFlag(castleFlag), enPassantFlag(enPassantFlag),
        captureFlag(captureFlag), doublePushFlag(doublePushFlag),
        promotedPiece(promotedPiece) {}

  Move(int fromSquare) : fromSquare(fromSquare) {}

  void printMove() const {
    std::cout << "Piece: " << pieceToChar[piece]
              << " From: " << boardMap[fromSquare] << boardMap[toSquare];
    if (captureFlag)
      std::cout << " Capture";
    if (doublePushFlag)
      std::cout << " Pawn double push";
    if (promotedPiece)
      std::cout << " Pawn promotion to: " << pieceToChar[promotedPiece];
    if (castleFlag)
      std::cout << " Castle";
    if (enPassantFlag)
      std::cout << " En passant";
    std::cout << "\n";
  }
};

#endif // CHESS_CPP_MOVE_H

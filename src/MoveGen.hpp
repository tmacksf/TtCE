#ifndef TTCE_MOVEGEN_HPP
#define TTCE_MOVEGEN_HPP
#include "GameState.hpp"

namespace TtCE {

class moveGen {
public:
  moveGen() = default;

  // Handles castling for the kingMoves
  template <Color turn>
  static void kingCastling(const int kingIndex, const gameState &gs,
                           std::vector<Move> &moves) {
    constexpr int kingSideIndex = (turn == BLACK ? 2 : 0);
    constexpr int queenSideIndex = (turn == BLACK ? 3 : 1);
    // todo make sure this is right
    constexpr BB kingSideOccupancy =
        (turn == BLACK ? (3ULL << 57) : (3ULL << 1));
    constexpr BB queenSideOccupancy =
        (turn == BLACK ? (7ULL << 60) : (7ULL << 4));
    // not sure how to better do this but for now this is okay. Square 1 is the
    // one that the king moves to on a castle
    constexpr int kingSideSquareOne = (turn == BLACK ? g8 : g1);
    constexpr int kingSideSquareTwo = (turn == BLACK ? f8 : f1);
    constexpr int queenSideSquareOne = (turn == BLACK ? c8 : c1);
    constexpr int queenSideSquareTwo = (turn == BLACK ? d8 : d1);
    int piece = gameState::getPiece(KING, turn);

    // TODO make sure that changing the availability of castling within this
    // method is okay checking to see if the rook is in the right position these
    // get the bitboard of the space where the rook should be on each side
    constexpr BB rookKingSide = (turn == BLACK ? 1ULL << 56 : 1ULL);
    constexpr BB rookQueenSide = (turn == BLACK ? 1ULL << 63 : 1ULL << 7);
    int rook = gameState::getPiece(ROOK, turn);
    BB rookBitboard = gs.getPieceBitboard(ROOK, turn);

    const BB allPieces = gs.allPieces();

    // king side castling
    if (gs.getCastling(kingSideIndex) and !(allPieces & (kingSideOccupancy))) {
      // are the squares attacked
      if (rookKingSide & rookBitboard) {
        if (!gs.attacked(kingSideSquareOne, gs.getAttacking()) and
            !gs.attacked(kingSideSquareTwo, gs.getAttacking())) {
          moves.emplace_back(kingIndex, kingSideSquareOne, piece, true, false,
                             false, false, false);
        }
      }
    }
    // queen side castling
    if (gs.getCastling(queenSideIndex) and !(allPieces & queenSideOccupancy)) {
      // are the squares attacked
      if (rookQueenSide & rookBitboard) {
        if (!gs.attacked(queenSideSquareOne, gs.getAttacking()) and
            !gs.attacked(queenSideSquareTwo, gs.getAttacking())) {
          moves.emplace_back(kingIndex, queenSideSquareOne, piece, true, false,
                             false, false, false);
        }
      }
    }
  }

  static void inline allPawnPromotions(int from, int to, int piece, int turn,
                                       bool capture, std::vector<Move> &moves) {
    moves.emplace_back(from, to, piece, false, false, capture, false,
                       QUEEN + 6 * turn);
    moves.emplace_back(from, to, piece, false, false, capture, false,
                       ROOK + 6 * turn);
    moves.emplace_back(from, to, piece, false, false, capture, false,
                       BISHOP + 6 * turn);
    moves.emplace_back(from, to, piece, false, false, capture, false,
                       KNIGHT + 6 * turn);
  }

  // TODO take the if (pawnPromotions and Type) out into another function
  // Section for templates
  template <Color turn, MoveType Type>
  static void pawnMoves(const gameState &gs, std::vector<Move> &moves) {
    constexpr BB promotionRank = (turn == BLACK ? rank2 : rank7);
    constexpr BB firstPushRank = (turn == BLACK ? rank6 : rank3);
    constexpr Direction direction = (turn == BLACK ? SOUTH : NORTH);
    constexpr Direction upRight = (turn == WHITE ? NORTH_EAST : SOUTH_WEST);
    constexpr Direction upLeft = (turn == WHITE ? NORTH_WEST : SOUTH_EAST);
    constexpr BB enPassantRank = (turn == BLACK ? rank3 : rank6);
    int piece = gameState::getPiece(PAWN, turn);

    const BB emptySquares = ~gs.allPieces();
    const BB pawns = gs.getPieceBitboard(PAWN, turn);
    const BB enemyBoard = gs.enemyBoard();

    // setting up for pawn promotions
    BB pawnPromotions = promotionRank & pawns;
    BB otherPawns = pawns & ~promotionRank;

    if (pawnPromotions && Type == Quiet) {
      BB promotions = Bitboard::shift<direction>(pawnPromotions) & emptySquares;
      while (promotions) {
        // handling promotions
        int to = pop_lsb(promotions);
        int from = to - direction;
        allPawnPromotions(from, to, piece, turn, false, moves);
      }
    }
    if (pawnPromotions && Type == Captures) {
      BB rightCapture = Bitboard::shift<upRight>(pawnPromotions) & enemyBoard;
      BB leftCapture = Bitboard::shift<upLeft>(pawnPromotions) & enemyBoard;
      while (rightCapture) {
        int to = pop_lsb(rightCapture);
        int from = to - upRight;
        allPawnPromotions(from, to, piece, turn, true, moves);
      }
      while (leftCapture) {
        int to = pop_lsb(leftCapture);
        int from = to - upLeft;
        allPawnPromotions(from, to, piece, turn, true, moves);
      }
    }
    /*if (pawnPromotions && Type == Evasions) {
      BB rightCapture =
          Bitboard::shift<upRight>(pawnPromotions) & gs.getAttackingPieces();
      BB leftCapture =
          Bitboard::shift<upLeft>(pawnPromotions) & gs.getAttackingPieces();
      BB promotions =
          Bitboard::shift<direction>(pawnPromotions) & gs.getAttackingPieces();

      int from;
      while (rightCapture) {
        int to = pop_lsb(rightCapture);
        from = to - upRight;
        allPawnPromotions(from, to, piece, turn, true, moves);
      }
      while (leftCapture) {
        int to = pop_lsb(leftCapture);
        from = to - upLeft;
        allPawnPromotions(from, to, piece, turn, true, moves);
      }
      while (promotions) {
        int to = pop_lsb(promotions);
        from = to - direction;
        allPawnPromotions(from, to, piece, turn, false, moves);
      }
    }
    */

    if constexpr (Type == Captures) {
      BB rightCapture = Bitboard::shift<upRight>(otherPawns) & enemyBoard;
      BB leftCapture = Bitboard::shift<upLeft>(otherPawns) & enemyBoard;

      while (rightCapture) {
        int to = pop_lsb(rightCapture);
        moves.emplace_back(to - upRight, to, piece, false, false, true, false,
                           false);
      }
      while (leftCapture) {
        int to = pop_lsb(leftCapture);
        moves.emplace_back(to - upLeft, to, piece, false, false, true, false,
                           false);
      }

      // EPs don't count as regular captures but a piece is still removed in
      // make move function thanks to eps flag
      if (gs.getEPSquare() != -1) {
        // TODO add assert that its on the (relative) 6th rank
        assert(gs.getEPSquare() < 64 and gs.getEPSquare() >= 0);
        BB epPawns = Bitboard::pawnAttacks[~turn][gs.getEPSquare()] & pawns;
        while (epPawns) {
          moves.emplace_back(pop_lsb(epPawns), gs.getEPSquare(), piece, false,
                             true, false, false, false);
        }
      }
    }

    if constexpr (Type == Quiet) {
      BB firstPush = Bitboard::shift<direction>(otherPawns) & emptySquares;
      BB secondPush =
          Bitboard::shift<direction>(firstPush & firstPushRank) & emptySquares;

      while (firstPush) {
        int to = pop_lsb(firstPush);
        moves.emplace_back(to - direction, to, piece, false, false, false,
                           false, false);
      }

      while (secondPush) {
        int to = pop_lsb(secondPush);
        moves.emplace_back(to - direction * 2, to, piece, false, false, false,
                           true, false);
      }
    }

    /*
    if constexpr (Type == Evasions) {
      BB firstPush = Bitboard::shift<direction>(otherPawns) & emptySquares;
      BB secondPush = Bitboard::shift<direction>(firstPush) &
                      (gs.getAttackingPieces() & emptySquares);
      firstPush &= gs.getAttackingPieces();
      BB rightCapture = Bitboard::shift<upRight>(otherPawns) &
                        (enemyBoard & gs.getAttackingPieces());
      BB leftCapture = Bitboard::shift<upLeft>(otherPawns) &
                       (enemyBoard & gs.getAttackingPieces());

      if (gs.getEPSquare() != -1) {
        if ((1ULL << gs.getEPSquare()) & gs.getAttackingPieces()) {
          BB epPawns = Bitboard::pawnAttacks[~turn][gs.getEPSquare()] & pawns;
          epPawns &= gs.getAttackingPieces();
          while (epPawns) {
            moves.emplace_back(pop_lsb(epPawns), gs.getEPSquare(), piece, false,
                               true, false, false, false);
          }
        }
      }

      while (firstPush) {
        int to = pop_lsb(firstPush);
        moves.emplace_back(to - direction, to, piece, false, false, false,
                           false, false);
      }
      while (secondPush) {
        int to = pop_lsb(secondPush);
        moves.emplace_back(to - direction * 2, to, piece, false, false, false,
                           true, false);
      }
      while (rightCapture) {
        int to = pop_lsb(rightCapture);
        moves.emplace_back(to - upRight, to, piece, false, false, true, false,
                           false);
      }
      while (leftCapture) {
        int to = pop_lsb(leftCapture);
        moves.emplace_back(to - upLeft, to, piece, false, false, true, false,
                           false);
      }
    }
    */

    if constexpr (Type == All) {
      pawnMoves<turn, Captures>(gs, moves);
      pawnMoves<turn, Quiet>(gs, moves);
    }
  }

  template <PieceTypes pt, MoveType Type>
  static void pieceMoves(const gameState &gs, std::vector<Move> &moves) {
    Color turn = gs.getTurn();
    BB pieceBitboard = gs.getPieceBitboard(pt, turn);
    BB enemyPieces = gs.enemyBoard();
    BB friendlyPieces = gs.friendlyBoard();
    BB allPieces =
        gs.friendlyBoard() |
        gs.enemyBoard(); // slightly faster than using gs.allPieces();
    int piece = gameState::getPiece(pt, turn);

    if constexpr (Type == Captures) {
      // TODO put the from/to inside the emplace back to reduce time
      while (pieceBitboard) {
        int from = pop_lsb(pieceBitboard);
        BB attacks = attacksBitboard<pt>(from, allPieces) & enemyPieces;
        while (attacks) {
          int to = pop_lsb(attacks);
          moves.emplace_back(from, to, piece, false, false, true, false, false);
        }
      }
    }

    if constexpr (Type == Quiet) {
      while (pieceBitboard) {
        int from = pop_lsb(pieceBitboard);
        // todo make sure that this works for generating possible moves
        BB attacks = attacksBitboard<pt>(from, allPieces) & ~allPieces;
        while (attacks) {
          int to = pop_lsb(attacks);
          moves.emplace_back(from, to, piece, false, false, false, false,
                             false);
        }
      }
    }

    /*
    if constexpr (Type == Evasions) {
      bool capture = false;
      while (pieceBitboard) {
        int from = pop_lsb(pieceBitboard);
        BB pieceMoves =
            attacksBitboard<pt>(from, allPieces) & gs.getAttackingPieces();
        while (pieceMoves) {
          int to = pop_lsb(pieceMoves);
          if ((1ULL << to) & enemyPieces)
            capture = true;
          moves.emplace_back(from, to, piece, false, false, capture, false,
                             false);
          capture = false;
        }
      }
    }
    */

    if constexpr (Type == All) {
      bool capture = false;
      while (pieceBitboard) {
        const int from = pop_lsb(pieceBitboard);

        BB attacks = attacksBitboard<pt>(from, allPieces) & ~friendlyPieces;

        while (attacks) {
          int to = pop_lsb(attacks);

          if ((1ULL << to) & enemyPieces)
            capture = true;
          //((1ULL << to) & enemyPieces) ? capture = true : capture = false;
          moves.emplace_back(from, to, piece, false, false, capture, false,
                             false);
          capture = false;
        }
      }
    }
  }

  template <Color Turn, MoveType Type>
  static void kingMoves(const gameState &gs, std::vector<Move> &moves) {
    BB kingBoard = gs.getPieceBitboard(KING, Turn);
    int kingPosition = Bitboard::getLeastSignificantBit(kingBoard);
    BB enemyBoard = gs.enemyBoard();
    BB friendlyBoard = gs.friendlyBoard();
    BB allPieces = friendlyBoard | enemyBoard;

    if constexpr (Type == Captures) {
      BB kingMoves = Bitboard::kingMoves[kingPosition] & enemyBoard;
      while (kingMoves) {
        int toSquare = pop_lsb(kingMoves);
        moves.emplace_back(kingPosition, toSquare, KING + 6 * Turn, false,
                           false, true, false, false);
      }
    }

    if constexpr (Type == Quiet) {
      BB kingMoves = Bitboard::kingMoves[kingPosition] & ~allPieces;
      while (kingMoves) {
        int toSquare = pop_lsb(kingMoves);
        moves.emplace_back(kingPosition, toSquare, KING + 6 * Turn, false,
                           false, false, false, false);
      }

      // castling
      if (!gs.isKingInCheck(gs.getAttacking()))
        kingCastling<Turn>(kingPosition, gs, moves);
    }

    if constexpr (Type == Evasions) {
      // could (Should) add some more to this like not moving into check
      kingMoves<Turn, Captures>(gs, moves);
      kingMoves<Turn, Quiet>(gs, moves);
    }

    if constexpr (Type == All) {
      kingMoves<Turn, Captures>(gs, moves);
      kingMoves<Turn, Quiet>(gs, moves);
    }
  }

  template <PieceTypes pt> static BB attacksBitboard(int square, BB blockers) {
    static_assert(pt != PAWN && pt != KING,
                  "Invalid piece type passed to attacks bitboard");
    switch (pt) {
    case QUEEN:
      return Magics::getQueenAttacks(square, blockers);
    case ROOK:
      return Magics::getRookAttacks(square, blockers);
    case BISHOP:
      return Magics::getBishopAttacks(square, blockers);
    case KNIGHT:
      return Bitboard::knightMoves[square];
    }
  }

  template <MoveType Type>
  static void pseudoLegalMoves(const gameState &gs, std::vector<Move> &moves) {
    if constexpr (Type == All) {
      Color Turn;
      if (gs.getTurn()) {
        pawnMoves<BLACK, Captures>(gs, moves);
        pieceMoves<KNIGHT, Captures>(gs, moves);
        pieceMoves<BISHOP, Captures>(gs, moves);
        pieceMoves<ROOK, Captures>(gs, moves);
        pieceMoves<QUEEN, Captures>(gs, moves);
        kingMoves<BLACK, Captures>(gs, moves);

        pawnMoves<BLACK, Quiet>(gs, moves);
        pieceMoves<KNIGHT, Quiet>(gs, moves);
        pieceMoves<BISHOP, Quiet>(gs, moves);
        pieceMoves<ROOK, Quiet>(gs, moves);
        pieceMoves<QUEEN, Quiet>(gs, moves);
        kingMoves<BLACK, Quiet>(gs, moves);
      } else {
        pawnMoves<WHITE, Captures>(gs, moves);
        pieceMoves<KNIGHT, Captures>(gs, moves);
        pieceMoves<BISHOP, Captures>(gs, moves);
        pieceMoves<ROOK, Captures>(gs, moves);
        pieceMoves<QUEEN, Captures>(gs, moves);
        kingMoves<WHITE, Captures>(gs, moves);

        pawnMoves<WHITE, Quiet>(gs, moves);
        pieceMoves<KNIGHT, Quiet>(gs, moves);
        pieceMoves<BISHOP, Quiet>(gs, moves);
        pieceMoves<ROOK, Quiet>(gs, moves);
        pieceMoves<QUEEN, Quiet>(gs, moves);
        kingMoves<WHITE, Quiet>(gs, moves);
      }
    }

    if constexpr (Type != All) {
      if (gs.getTurn()) {
        pawnMoves<BLACK, Type>(gs, moves);
        kingMoves<BLACK, Type>(gs, moves);
      } else {
        pawnMoves<WHITE, Type>(gs, moves);
        kingMoves<WHITE, Type>(gs, moves);
      }
      pieceMoves<QUEEN, Type>(gs, moves);
      pieceMoves<ROOK, Type>(gs, moves);
      pieceMoves<BISHOP, Type>(gs, moves);
      pieceMoves<KNIGHT, Type>(gs, moves);
    }
  }

  template <MoveType Type>
  static void legalMoves(const gameState &gs, std::vector<Move> &moves) {
    // TODO: make is so illegal moves are popped from the vector. Writing such a
    // large amount of moves is expensive so this can be remedied by popping
    // from vector
    int isKingInCheck = gs.isKingInCheck(gs.getAttacking());

    std::vector<Move> pseudoLegalMoveVector;

    if (isKingInCheck == 2) {
      if (gs.getTurn())
        kingMoves<BLACK, Type>(gs, pseudoLegalMoveVector);
      else
        kingMoves<WHITE, Type>(gs, pseudoLegalMoveVector);
      for (Move move : pseudoLegalMoveVector) {
        gameState gsCopy = gs;
        gsCopy.makeMove(move, 0ULL);
        if (!gsCopy.isKingInCheck(gs.getAttacking()))
          moves.push_back(move);
      }
    } else if (isKingInCheck == 1) {
      // EVASIONS IS NOT WORKING ATM
      /*gs.findAttackingLocation(); // finds the attackers
      if constexpr (Type == All) {
        pseudoLegalMoves<Evasions>(gs, pseudoLegalMoveVector);
        moves.reserve(pseudoLegalMoveVector.size());
        for (Move move : pseudoLegalMoveVector) {
          gameState gsCopy = gs;
          gsCopy.makeMove(move);
          if (!gsCopy.isKingInCheck(gs.getAttacking()))
            moves.push_back(move);
        }
      }

      if constexpr (Type != All) {
        pseudoLegalMoves<Type>(gs, pseudoLegalMoveVector);
        moves.reserve(pseudoLegalMoveVector.size());
        for (Move move : pseudoLegalMoveVector) {
          gameState gsCopy = gs;
          // gsCopy.findAttackingLine
          gsCopy.makeMove(move);
          if (!gsCopy.isKingInCheck(gs.getAttacking()))
            moves.push_back(move);
        }
      }*/
      pseudoLegalMoves<Type>(gs, pseudoLegalMoveVector);
      moves.reserve(pseudoLegalMoveVector.size());
      for (Move move : pseudoLegalMoveVector) {
        gameState gsCopy = gs;
        gsCopy.makeMove(move, 0ULL);
        if (!gsCopy.isKingInCheck(gs.getAttacking()))
          moves.push_back(move);
      }
    } else {
      pseudoLegalMoves<Type>(gs, pseudoLegalMoveVector);
      moves.reserve(pseudoLegalMoveVector.size());
      for (Move move : pseudoLegalMoveVector) {
        gameState gsCopy = gs;
        gsCopy.makeMove(move, 0ULL);
        if (!gsCopy.isKingInCheck(gs.getAttacking()))
          moves.push_back(move);
      }
    }
  }

  static void mg2(const gameState &gs, std::vector<Move> &moves) {
    std::vector<Move> pseudoLegalMoveVector;
    pseudoLegalMoves<All>(gs, pseudoLegalMoveVector);
    moves.reserve(pseudoLegalMoveVector.size());
    for (Move move : pseudoLegalMoveVector) {
      gameState gsCopy = gs;
      gsCopy.makeMove(move, 0ULL);
      if (!gsCopy.isKingInCheck(gs.getAttacking()))
        moves.push_back(move);
    }
  }

  static inline void sortMoves(const gameState &gs, std::vector<Move> &moves) {
    for (Move &m : moves) {
      m.scoreMove(gs.getEnemyPieceAt(m.m_toSquare), gs.getPly());
    }

    sort(moves.begin(), moves.end(), [](const Move &lhs, const Move &rhs) {
      return lhs.m_moveScore > rhs.m_moveScore;
    });
  }

  // we use this function to simultaniously check if the PV line continues and
  // score/sort the moves
  static inline bool sortMovesPV(const gameState &gs, std::vector<Move> &moves,
                                 Move &pvMove) {
    int pvFound = false;
    for (Move &m : moves) {
      if (m == pvMove) {
        m.pvMove();
        pvFound = true;
      } else {
        m.scoreMove(gs.getEnemyPieceAt(m.m_toSquare), gs.getPly());
      }
    }

    sort(moves.begin(), moves.end(), [](const Move &lhs, const Move &rhs) {
      return lhs.m_moveScore > rhs.m_moveScore;
    });

    return pvFound;
  }
};

} // namespace TtCE
#endif // TTCE_MOVEGEN_HPP

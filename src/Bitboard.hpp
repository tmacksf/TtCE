//
// Created by Thomas Mack on 23/12/2022.
//
#include "DefsEnums.hpp"

#ifndef TTCE_BITBOARDS_HPP
#define TTCE_BITBOARDS_HPP

namespace TtCE {

class Bitboard {
private:
  BB m_bitboard;

public:
  Bitboard() { m_bitboard = 0ULL; }

  [[nodiscard]] BB inline getValue() const { return m_bitboard; }

  // bitwise methods
  // todo transfer into this file to make them inline
  int getBitAt(int index) const;
  void setBitAt(int index);
  void unSetBitAt(int index);
  static void inline unSetBitAt(BB &bBoard, int index) {
    bBoard &= ~(1ULL << index);
  }
  void toggleBit(int index);
#if defined(__WIN32__)
  static int inline getLeastSignificantBit(BB b) {
    return unsigned __int64_tzcnt_u64(b);
  }
  int inline getLeastSignificantBit() const {
    return unsigned __int64_tzcnt_u64(m_bitboard);
  }
#else
  static int inline getLeastSignificantBit(BB b) { return __builtin_ctzll(b); }
  [[nodiscard]] int inline getLeastSignificantBit() const {
    return __builtin_ctzll(m_bitboard);
  }
#endif
  [[nodiscard]] int bitCount() const;
  // overloaded bit count to take a bitboard as an argument. Done using another
  // method. Will test the speed of both of these later on
  static inline int bitCount(BB x) {
    x -= (x >> 1) &
         0x5555555555555555; // put count of each 2 bits into those 2 bits
    x = (x & 0x3333333333333333) +
        ((x >> 2) &
         0x3333333333333333); // put count of each 4 bits into those 4 bits
    x = (x + (x >> 4)) &
        0x0f0f0f0f0f0f0f0f; // put count of each 8 bits into those 8 bits
    return ((x * 0x0101010101010101) >> 56);
  }
  // pretty printing of the bitboard
  void printBitboard() const;

  // static attack tables to do speedy move generation all but the pawn ones are
  // the same for both colors
  static BB kingMoves[64];
  static BB knightMoves[64];
  static BB pawnMoves[2][64];
  static BB pawnAttacks[2][64];

  // methods to initialize attack tables
  static void initAttackTables();
  static void initPawnAttacks();
  static void initPawnMoves();
  static void initKnightAttacks();
  static void initKingAttacks();

  static BB rayAttack(Direction direction, int index, int depth);

  // Section for templates
  // TODO add other directions for other move types
  template <Direction D> static constexpr BB shift(BB b) {
    return D == NORTH        ? b << 8
           : D == SOUTH      ? b >> 8
           : D == SOUTH_WEST ? (b & ~aFile) >> 7
           : D == SOUTH_EAST ? (b & ~hFile) >> 9
           : D == NORTH_EAST ? (b & ~hFile) << 7
           : D == NORTH_WEST ? (b & ~aFile) << 9
                             : 0;
  }
};
}; // namespace TtCE

#endif // TTCE_BITBOARDS_H

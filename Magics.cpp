//
// Created by Thomas Mack on 29/01/2023.
//

#include "Magics.h"

BB Magics::rookMask[64];
BB Magics::bishopMask[64];
BB Magics::rookMagics[64];
BB Magics::bishopMagics[64];
BB Magics::rookAttacks[64][4096];
BB Magics::bishopAttacks[64][512];

BB Magics::setOccupancy(int index, BB attackMask) {
  BB occupancy = 0ULL;
  int numBitsInMask = Bitboard::bitCount(attackMask);
  for (int i = 0; i < numBitsInMask; i++) {
    int square = Bitboard::getLeastSignificantBit(attackMask);
    Bitboard::unSetBitAt(attackMask, square);
    if (index & (1 << i)) {
      occupancy |= (1ULL << square);
    }
  }
  return occupancy;
}

void Magics::initRookMasks() {
  BB attacks;

  for (int i = 0; i < 64; i++) {
    attacks = 0Ull;

    int r, f;

    int tr = i / 8;
    int tf = i % 8;

    for (r = tr + 1; r <= 6; r++)
      attacks |= (1ULL << (r * 8 + tf));
    for (r = tr - 1; r >= 1; r--)
      attacks |= (1ULL << (r * 8 + tf));
    for (f = tf + 1; f <= 6; f++)
      attacks |= (1ULL << (tr * 8 + f));
    for (f = tf - 1; f >= 1; f--)
      attacks |= (1ULL << (tr * 8 + f));

    rookMask[i] = attacks;
  }
}

void Magics::initBishopMasks() {
  BB moves;
  BB position;
  for (int rank = 0; rank < 8; rank++) {
    for (int file = 0; file < 8; file++) {
      moves = 0ULL;
      int index = rank * 8 + file;
      position = 1ULL << index;

      if (rank >= file) {
        // shifting the predefined diagonal which is the bottom right to top
        // left diagonal to get north-west moves
        moves |= rightLeft << 8 * (rank - file);
      } else {
        moves |= rightLeft >> 8 * (file - rank);
      }

      int extra = 7 - rank;
      if (extra >= file) {
        moves |= leftRight >> 8 * (extra - file);
      } else {
        moves |= leftRight << 8 * (file - extra);
      }
      moves ^= position;
      moves &= outerRing;
      bishopMask[index] = moves;
    }
  }
}

BB Magics::randomBB() {
  BB u1, u2, u3, u4;
  u1 = (BB)(random()) & 0xFFFF;
  u2 = (BB)(random()) & 0xFFFF;
  u3 = (BB)(random()) & 0xFFFF;
  u4 = (BB)(random()) & 0xFFFF;
  return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}

BB Magics::randomBBFewBits() { return randomBB() & randomBB() & randomBB(); }

BB Magics::rookAttacksOnTheFly(int square, BB blockers) {
  BB result = 0ULL;

  int rank = square / 8;
  int file = square % 8;
  int r, f;

  for (r = rank + 1; r < 8; r++) {
    result |= 1ULL << (r * 8 + file);
    if (blockers & (1ULL << (r * 8 + file)))
      break;
  }

  for (r = rank - 1; r >= 0; r--) {
    result |= 1ULL << (r * 8 + file);
    if (blockers & (1ULL << (r * 8 + file)))
      break;
  }

  for (f = file + 1; f < 8; f++) {
    result |= (1ULL << (rank * 8 + f));
    if (blockers & (1ULL << (rank * 8 + f)))
      break;
  }

  for (f = file - 1; f >= 0; f--) {
    result |= (1ULL << (rank * 8 + f));
    if (blockers & (1ULL << (rank * 8 + f)))
      break;
  }

  return result;
}

BB Magics::bishopAttacksOnTheFly(int square, BB blockers) {
  BB result = 0ULL;

  int rank = square / 8;
  int file = square % 8;
  int r, f;

  // northwest (+rank, +file)
  for (r = rank + 1, f = file + 1; r < 8 && f < 8; r++, f++) {
    result |= (1ULL << (r * 8 + f));
    if (blockers & (1ULL << (r * 8 + f)))
      break;
  }

  // northeast (+rank, -file)
  for (r = rank + 1, f = file - 1; r < 8 && f >= 0; r++, f--) {
    result |= (1ULL << (r * 8 + f));
    if (blockers & (1ULL << (r * 8 + f)))
      break;
  }

  // southwest (-rank, +file)
  for (r = rank - 1, f = file + 1; r >= 0 && f < 8; r--, f++) {
    result |= (1ULL << (r * 8 + f));
    if (blockers & (1ULL << (r * 8 + f)))
      break;
  }

  for (r = rank - 1, f = file - 1; r >= 0 && f >= 0; r--, f--) {
    result |= (1ULL << (r * 8 + f));
    if (blockers & (1ULL << (r * 8 + f)))
      break;
  }

  return result;
}

void Magics::generateMagicNumbers() {
  initBishopMasks();
  initRookMasks();
  for (int i = 0; i < 64; i++) {
    rookMagics[i] = findRookMagic(i, rookBitCounts[i]);
    bishopMagics[i] = findBishopMagic(i, bishopBitCounts[i]);
  }
}

BB Magics::findRookMagic(int sq, int numBits) {
  BB attackMask;
  BB occupancies[4096];
  BB attacks[4096];
  BB usedAttacks[4096];
  BB magic;
  int i, randomCount, maskBitCount, fail;
  int magicIndex;

  attackMask = rookMask[sq];
  maskBitCount = Bitboard::bitCount(attackMask);

  for (i = 0; i < (1 << maskBitCount); i++) {
    occupancies[i] = setOccupancy(i, attackMask);
    attacks[i] = rookAttacksOnTheFly(sq, occupancies[i]);
  }

  for (randomCount = 0; randomCount < 100000000; randomCount++) {
    magic = randomBBFewBits();
    if (Bitboard::bitCount((attackMask * magic) & 0xFF00000000000000ULL) < 6)
      continue;

    for (i = 0; i < 4096; i++)
      usedAttacks[i] = 0ULL;
    for (i = 0, fail = 0; !fail && i < (1 << maskBitCount); i++) {
      magicIndex = (int)((occupancies[i] * magic) >> (64 - numBits));
      if (usedAttacks[magicIndex] == 0ULL)
        usedAttacks[magicIndex] = attacks[i];
      else if (usedAttacks[magicIndex] != attacks[i])
        fail = 1;
    }
    if (!fail)
      return magic;
  }
  printf("***Failed***\n");
  return 0ULL;
}

BB Magics::findBishopMagic(int sq, int numBits) {
  BB attackMask;
  BB occupancies[4096];
  BB attacks[4096];
  BB usedAttacks[4096];
  BB magic;
  int i, randomCount, maskBitCount, fail;
  int magicIndex;

  attackMask = bishopMask[sq];
  maskBitCount = Bitboard::bitCount(attackMask);

  for (i = 0; i < (1 << maskBitCount); i++) {
    occupancies[i] = setOccupancy(i, attackMask);
    attacks[i] = bishopAttacksOnTheFly(sq, occupancies[i]);
  }

  for (randomCount = 0; randomCount < 100000000; randomCount++) {
    magic = randomBBFewBits();
    if (Bitboard::bitCount((attackMask * magic) & 0xFF00000000000000ULL) < 6)
      continue;

    for (i = 0; i < 4096; i++)
      usedAttacks[i] = 0ULL;
    for (i = 0, fail = 0; !fail && i < (1 << maskBitCount); i++) {
      magicIndex = (int)((occupancies[i] * magic) >> (64 - numBits));
      if (usedAttacks[magicIndex] == 0ULL)
        usedAttacks[magicIndex] = attacks[i];
      else if (usedAttacks[magicIndex] != attacks[i])
        fail = 1;
    }
    if (!fail)
      return magic;
  }
  std::cout << "***Failed***\n";
  return 0ULL;
}

void Magics::initRookAttacks() {
  // loop through every square on the board
  for (int square = 0; square < 64; square++) {
    BB mask = rookMask[square];
    int occupancyIndices = 1 << rookBitCounts[square];

    for (int i = 0; i < occupancyIndices; i++) {
      BB occ = setOccupancy(i, mask);
      rookAttacks[square][(occ * preCalcRookMagics[square]) >>
                          (64 - rookBitCounts[square])] =
          rookAttacksOnTheFly(square, occ);
      printBitString(rookAttacks[square][(occ * preCalcRookMagics[square]) >>
                                         (64 - rookBitCounts[square])]);
    }
  }
}

void Magics::initBishopAttacks() {
  for (int square = 0; square < 64; square++) {
    BB mask = bishopMask[square];
    int occupancyIndices = 1 << bishopBitCounts[square];

    for (int i = 0; i < occupancyIndices; i++) {
      BB occ = setOccupancy(i, mask);
      bishopAttacks[square][(occ * preCalcBishopMagics[square]) >>
                            (64 - bishopBitCounts[square])] =
          bishopAttacksOnTheFly(square, occ);
    }
  }
}

void Magics::generateSlidingAttackTables() {
  // initialize masks first
  initBishopMasks();
  initRookMasks();

  initRookAttacks();
  initBishopAttacks();
}

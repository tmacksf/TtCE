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

    int rank, file;

    int startRank = i / 8;
    int startFile = i % 8;

    for (rank = startRank + 1; rank <= 6; rank++) attacks |= (1ULL << (rank * 8 + startFile));
    for (rank = startRank - 1; rank >= 1; rank--) attacks |= (1ULL << (rank * 8 + startFile));
    for (file = startFile + 1; file <= 6; file++) attacks |= (1ULL << (startRank * 8 + file));
    for (file = startFile - 1; file >= 1; file--) attacks |= (1ULL << (startFile * 8 + file));

    rookMask[i] = attacks;
  }
}

void Magics::initBishopMasks() {
    BB attacks;
    for (int i = 0; i < 64; i++){
        attacks = 0ULL;

        int rank, file;

        int startRank = i/8;
        int startFile = i % 8;

        for (rank = startRank + 1, file = startFile + 1; rank <= 6 && file <= 6; rank++, file++) attacks |= (1ULL << (rank*8+file));
        for (rank = startRank + 1, file = startFile - 1; rank <= 6 && file >= 1; rank++, file--) attacks |= (1ULL << (rank*8+file));
        for (rank = startRank - 1, file = startFile + 1; rank >= 1 && file <= 6; rank--, file++) attacks |= (1ULL << (rank*8+file));
        for (rank = startRank - 1, file = startFile - 1; rank >= 1 && file >= 1; rank--, file--) attacks |= (1ULL << (rank*8+file));
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
      int magicIndex = (occ * preCalcRookMagics[square]) >> (64-rookBitCounts[square]);
      rookAttacks[square][magicIndex]= rookAttacksOnTheFly(square, occ);
    }
  }
}

void Magics::initBishopAttacks() {
  for (int square = 0; square < 64; square++) {
    BB mask = bishopMask[square];
    int occupancyIndices = 1 << bishopBitCounts[square];

    for (int i = 0; i < occupancyIndices; i++) {
      BB occ = setOccupancy(i, mask);
      int magicIndex = (occ * preCalcBishopMagics[square]) >> (64-bishopBitCounts[square]);
      bishopAttacks[square][magicIndex] =
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

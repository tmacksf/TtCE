//
// Created by Thomas Mack on 03/01/2023.
//

#include "Bitboard.h"

int Bitboard::getBitAt(int index) const {
    //assert(index >= 0 and index < 64);
    return (this->m_bitboard & (1ULL << index)) ? 1 : 0;
}

void Bitboard::setBitAt(int index) {
    //assert(index >= 0 and index < 64);
    this->m_bitboard |= 1ULL << index;
}

void Bitboard::unSetBitAt(int index) {
    //assert(index >= 0 and index < 64);
    // don't know if I need this checker to see if bit is positive. Might be redundant and slow it down too much
    if (getBitAt(index) == 1ULL) {
        this->m_bitboard &= ~(1ULL << index);
    }
}

void Bitboard::toggleBit(int index) {
    //assert(index >= 0 and index < 64);
    this->m_bitboard ^= 1ULL << index;
}

int Bitboard::bitCount() const{
    BB x = this->m_bitboard;
    x -= (x >> 1) & 0x5555555555555555; //put count of each 2 bits into those 2 bits
    x = (x & 0x3333333333333333) + ((x >> 2) & 0x3333333333333333); //put count of each 4 bits into those 4 bits
    x = (x + (x >> 4)) & 0x0f0f0f0f0f0f0f0f;  //put count of each 8 bits into those 8 bits
    return ((x * 0x0101010101010101) >> 56);  //returns left 8 bits of x + (x<<8) + (x<<16) + (x<<24) + ...
}

int Bitboard::getLeastSignificantBit() const {
    return __builtin_ctzll(this->m_bitboard);
}

void Bitboard::printBitboard() const {
    std::cout << "  a b c d e f g h" << std::endl;
    for (int file = 0; file < 8; file ++) {
        std::cout << 8-file;
        std::cout << " ";
        for (int rank = 0; rank < 8; rank ++) {
            std::cout << Bitboard::getBitAt(63 - file * 8 - rank);
            std::cout << ' ';
        }
        std::cout << std::endl;
    }
}

// initialize attack tables
BB Bitboard::kingMoves[64];
BB Bitboard::queenMoves[64];
BB Bitboard::rookMoves[64];
BB Bitboard::bishopMoves[64];
BB Bitboard::knightMoves[64];
BB Bitboard::pawnMoves[2][64];
BB Bitboard::pawnAttacks[2][64];

void Bitboard::initAttackTables() {
    Bitboard::initPawnMoves();
    Bitboard::initPawnAttacks();
    Bitboard::initKnightAttacks();
    Bitboard::initBishopAttacks();
    Bitboard::initRookAttacks();
    Bitboard::initQueenAttacks();
    Bitboard::initKingAttacks();
}

void Bitboard::initPawnAttacks() {
    BB attacks;
    for (int i = 0; i < boardSize; i++) {
        attacks = 0ULL;
        if (i < 8 or i >= 56) {
            continue;
        }
        // white pawn attacks
        (i % 8 == 0) ? : attacks |= 1ULL << (i + NORTH_EAST);
        ((i+1) % 8 == 0) ? : attacks |= 1ULL << (i + NORTH_WEST);
        Bitboard::pawnAttacks[WHITE][i];
        attacks = 0ULL;

        // black pawn attacks
        (i % 8 == 0) ? : attacks |= 1ULL << (i + SOUTH_EAST);
        ((i+1) % 8 == 0) ? : attacks |= 1ULL << (i + SOUTH_WEST);
        Bitboard::pawnAttacks[BLACK][i];
    }
}

void Bitboard::initPawnMoves(){
    BB moves;
    for (int i = 0; i < boardSize; i ++) {
        moves = 0ULL;
        if (i < 8 or i >= 56) {
            continue;
        }
        // white pawn moves
        moves |= 1ULL << (i + NORTH);
        (i >= 16) ? : moves |= 1ULL << (i + NORTH + NORTH);
        Bitboard::pawnMoves[WHITE][i] = moves;
        moves = 0ULL;

        // black pawn moves
        moves |= 1ULL << (i + SOUTH);
        (i < 48) ? : moves |= 1ULL << (i + SOUTH + SOUTH);
        Bitboard::pawnMoves[BLACK][i] = moves;
    }
}

void Bitboard::initKingAttacks() {
    BB moves;
    for (int index = 0; index < boardSize; index++) {
        moves = 0ULL;
        for (const auto d : allDirections) {
            // need to make sure we aren't going off board with this ugly bit of code
            if (
                    (index < 8 and (d == SOUTH or d == SOUTH_WEST or d == SOUTH_EAST)) or
                    (index % 8 == 0 and (d == EAST or d == SOUTH_EAST or d == NORTH_EAST)) or
                    (index+1) % 8 == 0 and (d == WEST or d == SOUTH_WEST or d == NORTH_WEST) or
                    (index >= 56 and (d == NORTH or d == NORTH_WEST or d == NORTH_EAST))
                )
            {
                continue;
            }
            moves |= Bitboard::rayAttack(d, index, 1);
        }
        Bitboard::kingMoves[index] = moves;
    }
}

void Bitboard::initRookAttacks() {
    BB moves;
    BB position;
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            moves = 0ULL;
            int index = rank * 8 + file;
            position = 1ULL << index;
            moves |= ranks[rank];
            moves |= files[file];
            moves ^= position;
            Bitboard::rookMoves[index] = moves;
        }
    }
}

void Bitboard::initBishopAttacks() {
    BB moves;
    BB position;
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            moves = 0ULL;
            int index = rank * 8 + file;
            position = 1ULL << index;

            if (rank >= file) {
                moves |= rightLeft << 8 * (rank - file);
            }
            else {
                moves |= rightLeft >> 8 * (file - rank);
            }

            int extra = 7 - rank;
            if (extra >= file) {
                moves |= leftRight >> 8 * (extra - file);
            }
            else {
                moves |= leftRight << 8 * (file - extra);
            }
            moves ^= position;
            Bitboard::bishopMoves[index] = moves;
        }
    }
}

BB Bitboard::rayAttack(Direction direction, int index, int depth) {
    // method to get a bitboard containing the ray attack for a specific direction.
    // direction: direction of attack, index: location of the piece, depth: depth of ray, meaning
    BB attacks = 0ULL;
    for (int i = 0; i < depth; i++) {
        attacks |= 1ULL << (index + direction * (i + 1));
    }
    return attacks;
}

void Bitboard::initQueenAttacks() {
    BB moves;
    BB position;

    for (int rank = 0; rank < 8; rank ++) {
        for (int file = 0; file < 8; file ++) {
            int index = rank * 8 + file;
            position = 0ULL;
            position |= 1ULL << index;
            moves = 0ULL;

            // diagonal moves
            if (rank >= file) {
                moves |= rightLeft << 8 * (rank - file);
            }
            else {
                moves |= rightLeft >> 8 * (file - rank);
            }

            int extra = 7 - rank;
            if (extra >= file) {
                moves |= leftRight >> 8 * (extra - file);
            }
            else {
                moves |= leftRight << 8 * (file - extra);
            }

            // square moves
            moves |= ranks[rank];
            moves |= files[file];

            moves ^= position;
            Bitboard::queenMoves[index] = moves;
        }
    }
}

void Bitboard::initKnightAttacks() {
    BB moves;
    BB position;

    for (int rank = 0; rank < 8; rank ++) {
        for (int file = 0; file < 8; file ++) {
            int index = rank * 8 + file;
            moves = 0ULL;

            // up 2 left 1
            if (rank < 6 and file != 7) moves |= 1ULL << (index + NORTH + NORTH_WEST);
            // up 2 right 1
            if (rank < 6 and file != 0) moves |= 1ULL << (index + NORTH + NORTH_EAST);
            // up 1 left 2
            if (rank < 7 and file < 6) moves |= 1ULL << (index + WEST + NORTH_WEST);
            // up 1 right 2
            if (rank < 7 and file > 1) moves |= 1ULL << (index + EAST + NORTH_EAST);

            // down 2 left 1
            if (rank > 1 and file != 7) moves |= 1ULL << (index + SOUTH + SOUTH_WEST);
            // down 2 right 1
            if (rank > 1 and file != 0) moves |= 1ULL << (index + SOUTH + SOUTH_EAST);
            // down 1 left 2
            if (rank > 0 and file < 6) moves |= 1ULL << (index + WEST + SOUTH_WEST);
            // down 1 right 2
            if (rank > 0 and file > 1) moves |= 1ULL << (index + EAST + SOUTH_EAST);

            Bitboard::knightMoves[index] = moves;
        }
    }
}
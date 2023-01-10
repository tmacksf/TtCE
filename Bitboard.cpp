//
// Created by Thomas Mack on 03/01/2023.
//

#include "Bitboard.h"

int Bitboard::getBitAt(int index) {
    assert(0 <= index < 64);
    return (this->m_bitboard & (1ULL << index)) ? 1 : 0;
}

void Bitboard::setBitAt(int index) {
    assert(0 <= index < 64);
    this->m_bitboard |= 1ULL << index;
}

void Bitboard::unSetBitAt(int index) {
    assert(0 <= index < 64);
    // don't know if I need this checker to see if bit is positive. Might be redundant and slow it down too much
    if (getBitAt(index) == 1ULL) {
        this->m_bitboard &= ~(1ULL << index);
    }
}

void Bitboard::toggleBit(int index) {
    assert(0 <= index < 64);
    this->m_bitboard ^= 1ULL << index;
}

int Bitboard::pieceCount(){
    // inefficient. Need to change to a better method
    int total = 0;
    for (int i = 0; i < 64; i++) {
        total += getBitAt(i);
    }
    return total;
}

void Bitboard::printBitboard() {
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

BB Bitboard::rookMoves[] = {0ULL};
BB Bitboard::whitePawnMoves[] = {0ULL};
BB Bitboard::blackPawnMoves[] = {0ULL};

void Bitboard::initializePawnAttacks() {
    // might want to split this up into pawn moves and attacks for easier move generation down the line, but we shall see
    // to make the index uniform might want to convert back to 64 array or make sure to add 8 when doing move generation
    BB attacks;
    int piece_index;
    for (int i = 0; i < boardSize-16; i++) {
        attacks = 0ULL;
        // white pawn moves/attacks
        piece_index = i + 8;
        (i % 8 == 0) ? : attacks |= 1ULL << (piece_index + NORTH_EAST);
        attacks |= 1ULL << (piece_index + NORTH);
        (piece_index >= 16) ? : attacks |= 1ULL << (piece_index + NORTH + NORTH);
        ((i+1) % 8 == 0) ? : attacks |= 1ULL << (piece_index + NORTH_WEST);
        Bitboard::whitePawnMoves[i] = attacks;
        attacks = 0ULL;

        // black pawn moves/attacks
        (i % 8 == 0) ? : attacks |= 1ULL << (piece_index + SOUTH_EAST);
        attacks |= 1ULL << (piece_index + SOUTH);
        (piece_index < 48) ? : attacks |= 1ULL << (piece_index + SOUTH + SOUTH);
        ((i+1) % 8 == 0) ? : attacks |= 1ULL << (piece_index + SOUTH_WEST);
        Bitboard::blackPawnMoves[i] = attacks;
    }
}
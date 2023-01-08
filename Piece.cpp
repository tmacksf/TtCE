//
// Created by Tommy on 1/8/2023.
//

#include "Piece.h"

Piece::Piece() {
    pieceChars[K] = 'K';
    pieceChars[k] = 'k';
    pieceChars[Q] = 'Q';
    pieceChars[q] = 'q';
    pieceChars[R] = 'R';
    pieceChars[r] = 'r';
    pieceChars[B] = 'B';
    pieceChars[Pieces(b)] = 'b';
    pieceChars[N] = 'N';
    pieceChars[n] = 'n';
    pieceChars[P] = 'P';
    pieceChars[Pieces(p)] = 'p';

    charsPiece['K'] = K;
    charsPiece['k'] = k;
    charsPiece['Q'] = Q;
    charsPiece['q'] = q;
    charsPiece['R'] = R;
    charsPiece['r'] = r;
    charsPiece['B'] = B;
    charsPiece['b'] = Pieces(b);
    charsPiece['N'] = N;
    charsPiece['n'] = n;
    charsPiece['P'] = P;
    charsPiece['p'] = Pieces(p);
}

char Piece::getChar(int p) {
    switch (p) {
        case 0:
            return 'K';
        case 1:
            return 'k';
        case 2:
            return 'Q';
        case 3:
            return 'q';
        case 4:
            return 'R';
        case 5:
            return 'r';
        case 6:
            return 'B';
        case 7:
            return 'b';
        case 8:
            return 'N';
        case 9:
            return 'n';
        case 10:
            return 'P';
        case 11:
            return 'p';
        default:
            throw std::invalid_argument("Value not valid piece");
            return -1;
    }
}

int Piece::getInt(char c) {
    switch (p) {
        case 'K':
            return 0;
        case 'k':
            return 1;
        case 'Q':
            return 2;
        case 'q':
            return 3;
        case 'R':
            return 4;
        case 'r':
            return 5;
        case 'B':
            return 6;
        case 'b':
            return 7;
        case 'N':
            return 8;
        case 'n':
            return 9;
        case 'P':
            return 10;
        case 'p':
            return 11;
        default:
            throw std::invalid_argument("Value not valid char for piece");
            return -1;
    }
}
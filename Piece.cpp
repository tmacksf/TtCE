//
// Created by Tommy on 1/8/2023.
//

#include "Piece.h"

Piece::Piece() {
    pieceChars[K] = 'K';

    pieceChars[Q] = 'Q';
    pieceChars[R] = 'R';
    pieceChars[B] = 'B';
    pieceChars[N] = 'N';
    pieceChars[P] = 'P';
    pieceChars[k] = 'k';
    pieceChars[q] = 'q';
    pieceChars[r] = 'r';
    pieceChars[Pieces(b)] = 'b';
    pieceChars[n] = 'n';
    pieceChars[Pieces(p)] = 'p';

    charsPiece['K'] = K;
    charsPiece['Q'] = Q;
    charsPiece['R'] = R;
    charsPiece['B'] = B;
    charsPiece['N'] = N;
    charsPiece['P'] = P;
    charsPiece['k'] = k;
    charsPiece['q'] = q;
    charsPiece['r'] = r;
    charsPiece['b'] = Pieces(b);
    charsPiece['n'] = n;
    charsPiece['p'] = Pieces(p);
}

char Piece::getChar(int p) {
    switch (p) {
        case 0:
            return 'K';
        case 1:
            return 'Q';
        case 2:
            return 'R';
        case 3:
            return 'B';
        case 4:
            return 'N';
        case 5:
            return 'P';
        case 6:
            return 'k';
        case 7:
            return 'q';
        case 8:
            return 'r';
        case 9:
            return 'b';
        case 10:
            return 'n';
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
        case 'Q':
            return 2;
        case 'R':
            return 4;
        case 'B':
            return 6;
        case 'N':
            return 8;
        case 'P':
            return 10;
        case 'k':
            return 1;
        case 'q':
            return 3;
        case 'r':
            return 5;
        case 'b':
            return 7;
        case 'n':
            return 9;
        case 'p':
            return 11;
        default:
            throw std::invalid_argument("Value not valid char for piece");
            return -1;
    }
}
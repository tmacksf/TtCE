//
// Created by Tommy on 1/8/2023.
//

#include "Piece.h"

Piece::Piece() {
    /*Piece::pieceChars[K] = 'K';
    Piece::pieceChars[Q] = 'Q';
    Piece::pieceChars[R] = 'R';
    Piece::pieceChars[B] = 'B';
    Piece::pieceChars[N] = 'N';
    Piece::pieceChars[P] = 'P';
    Piece::pieceChars[k] = 'k';
    Piece::pieceChars[q] = 'q';
    Piece::pieceChars[r] = 'r';
    Piece::pieceChars[Pieces(b)] = 'b';
    Piece::pieceChars[n] = 'n';
    Piece::pieceChars[Pieces(p)] = 'p';

    Piece::charsPiece['K'] = K;
    Piece::charsPiece['Q'] = Q;
    Piece::charsPiece['R'] = R;
    Piece::charsPiece['B'] = B;
    Piece::charsPiece['N'] = N;
    Piece::charsPiece['P'] = P;
    Piece::charsPiece['k'] = k;
    Piece::charsPiece['q'] = q;
    Piece::charsPiece['r'] = r;
    Piece::charsPiece['b'] = Pieces(b);
    Piece::charsPiece['n'] = n;
    Piece::charsPiece['p'] = Pieces(p);*/
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
    switch (c) {
        case 'K':
            return 0;
        case 'Q':
            return 1;
        case 'R':
            return 2;
        case 'B':
            return 3;
        case 'N':
            return 4;
        case 'P':
            return 5;
        case 'k':
            return 6;
        case 'q':
            return 7;
        case 'r':
            return 8;
        case 'b':
            return 9;
        case 'n':
            return 10;
        case 'p':
            return 11;
        default:
            throw std::invalid_argument("Value not valid char for piece");
            return -1;
    }
}
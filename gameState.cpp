//
// Created by Thomas Mack on 28/12/2022.
//

#include "gameState.h"

gameState::gameState() {
    this->turn = WHITE;
    for (int i = 0; i < 12; i++) {
        this->bitboards[i] = (0ULL);
    }
}

void gameState::initialise(std::string fen) {
    //splitting the string into two parts, first to define the board position and second to define castling etc.
    int i = 0;
    std::string boardPosition;

    while (fen[i] != ' ') {
        boardPosition.append(1, fen[i]);
        i++;
    }
    std::string extraStuff(fen, i, fen.length());

    int boardPos = 63;
    for (i = 0; i < boardPosition.length(); i++) {
        if (std::isdigit(fen[i])) {
            //not sure if I should be casting like this
            boardPos -= (int)fen[i];
        }
        else {
            switch (boardPosition[i]) {
                case 'K':
                    this->bitboards[0].setBitAt(boardPos);
                    break;
                case 'k':
                    this->bitboards[1].setBitAt(boardPos);
                    break;
                case 'Q':
                    this->bitboards[2].setBitAt(boardPos);
                    break;
                case 'q':
                    this->bitboards[3].setBitAt(boardPos);
                    break;
                case 'R':
                    this->bitboards[4].setBitAt(boardPos);
                    break;
                case 'r':
                    this->bitboards[5].setBitAt(boardPos);
                    break;
                case 'N':
                    this->bitboards[6].setBitAt(boardPos);
                    break;
                case 'n':
                    this->bitboards[7].setBitAt(boardPos);
                    break;
                case 'B':
                    this->bitboards[8].setBitAt(boardPos);
                    break;
                case 'b':
                    this->bitboards[9].setBitAt(boardPos);
                    break;
                case 'P':
                    this->bitboards[10].setBitAt(boardPos);
                    break;
                case 'p':
                    this->bitboards[11].setBitAt(boardPos);
                    break;
                default:
                    // not sure if I need this, but it is to make sure the counting does not go up with / and spaces
                    boardPos += 1;
                    break;
            }
            boardPos -= 1;
        }
    }

    for (i = 0; i < extraStuff.length(); i++) {
        switch (extraStuff[i]) {
            case ('w'):
                this->turn = WHITE;
                break;
            case ('b'):
                this->turn = BLACK;
                break;
            case ('K'):
                this->castling[0] = true;
                break;
            case ('Q'):
                this->castling[1] = true;
                break;
            case ('k'):
                this->castling[2] = true;
                break;
            case ('q'):
                this->castling[3] = true;
                break;
            default:
                break;
        }
    }
}

void gameState::printing() {

}
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
    std::string castling;
    std::string turn;
    std::string enPassant;

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
            // could do all this with the piece index so might implement that later with a pointer to the piece object in the game class

            switch (boardPosition[i]) {
                case 'K':
                    this->bitboards[0].setBitAt(boardPos);
                    break;
                case 'Q':
                    this->bitboards[1].setBitAt(boardPos);
                    break;
                case 'R':
                    this->bitboards[2].setBitAt(boardPos);
                    break;
                case 'B':
                    this->bitboards[3].setBitAt(boardPos);
                    break;
                case 'N':
                    this->bitboards[4].setBitAt(boardPos);
                    break;
                case 'P':
                    this->bitboards[5].setBitAt(boardPos);
                    break;
                case 'k':
                    this->bitboards[6].setBitAt(boardPos);
                    break;
                case 'q':
                    this->bitboards[7].setBitAt(boardPos);
                    break;
                case 'r':
                    this->bitboards[8].setBitAt(boardPos);
                    break;
                case 'b':
                    this->bitboards[9].setBitAt(boardPos);
                    break;
                case 'n':
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
                this->attacking = BLACK;
                break;
            case ('b'):
                this->turn = BLACK;
                this->attacking = WHITE;
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

void gameState::printing(Piece pcs) {
    std::string outString;
    int counter = 0;
    for (int i = 63; i >= 0; i--) {
        int flag = 0;
        for (int j = 0; j < 12; j ++) {
            if (this->bitboards[j].getBitAt(i) == 1) {
                outString.append(1, pcs.getChar(j));
                flag = 1;
            }
        }
        if (!flag) {
            outString.append(1, '-');
        }
        counter += 1;
        if (counter == 8) {
            std::cout << outString << std::endl;
            counter = 0;
            outString = "";
        }
    }
}

BB gameState::allPieces() {
    BB all = 0ULL;
    for (int i = 0; i < 12; i++) {
        all |= this->bitboards[i].getValue();
    }
    return all;
}

BB gameState::friendlyBoard() {
    BB friendly = 0ULL;
    for (int i = 0; i < 6; i++) {
        friendly |= this->bitboards[i+this->turn].getValue();
    }
    return friendly;
}

BB gameState::enemyBoard() {
    BB enemy = 0ULL;
    for (int i = 0; i < 6; i++){
        enemy |= this->bitboards[i+this->attacking].getValue();
    }
    return enemy;
}
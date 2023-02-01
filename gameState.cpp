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

    std::string s;
    std::stringstream ss(fen);
    std::vector<std::string> fenSeperated;
    while (std::getline(ss, s, ' ')) {
        fenSeperated.push_back(s);
    }

    // board position section
    int boardPos = 63;
    for (i = 0; i < fenSeperated[0].length(); i++) {
        if (std::isdigit(fenSeperated[0][i])) {
            //not sure if I should be casting like this
            boardPos -= int(fenSeperated[0][i]) - 48;
        }
        else if (fen[i] == '/') { ;
        } else {
            char temp = fenSeperated[0][i];
            bitboards[charToPiece[temp]].setBitAt(boardPos);
            boardPos -= 1;
        }
    }

    // turn section
    if (fenSeperated[1][0] == 'w'){
        this->turn = WHITE;
    }
    else {
        this->turn = BLACK;
    }

    // castling section
    for (i = 0; i < fenSeperated[2].length(); i++){
        if (fenSeperated[2][i] == 'Q') this->castling[1] = true;
        if (fenSeperated[2][i] == 'K') this->castling[0] = true;
        if (fenSeperated[2][i] == 'k') this->castling[2] = true;
        if (fenSeperated[2][i] == 'q') this->castling[3] = true;
    }

    int enPSquare;
    for (i = 0; i < fenSeperated[3].length(); i++) {
        // TODO need to add en passant square
    }
    this->enPassantSquare = enPSquare;

    // TODO half move counter and full move counter
}

void gameState::printing() {
    std::string outString;
    int counter = 0;
    for (int i = 63; i >= 0; i--) {
        int flag = 0;
        for (int j = 0; j < 12; j ++) {
            if (this->bitboards[j].getBitAt(i) == 1) {
                outString.append(1, pieceToChar[j]);
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
        friendly |= this->bitboards[i+6*this->turn].getValue();
    }
    return friendly;
}

BB gameState::enemyBoard() {
    BB enemy = 0ULL;
    for (int i = 0; i < 6; i++){
        enemy |= this->bitboards[i+6*this->attacking].getValue();
    }
    return enemy;
}


// this does not have to be done until after move generation but had ideas so might as well do it now
void gameState::moveToGameState(Move &move) {
    // done on a copy of the parent game state to create a child game state based on a move
    bitboards[move.piece].unSetBitAt(move.fromSquare);
    bitboards[move.piece].setBitAt(move.toSquare);
    if (move.castleFlag) {
        // set the castle index
    }
    if (move.enPassantFlag) {
        // this->enPassantSquare = ;
    }
}
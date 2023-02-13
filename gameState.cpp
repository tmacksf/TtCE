//
// Created by Thomas Mack on 28/12/2022.
//

#include "gameState.h"

gameState::gameState() {
    this->turn = WHITE;
    for (int i = 0; i < 12; i++) {
        bitboards[i] = Bitboard();
    }
}

void gameState::initialise(std::string fen) {
    //splitting the string into two parts, first to define the board position and second to define castling etc.
    int i;

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
        this->attacking = BLACK;
    }
    else {
        this->turn = BLACK;
        this->attacking = WHITE;
    }

    // castling section
    castling[0] = false;
    castling[1] = false;
    castling[2] = false;
    castling[3] = false;
    for (i = 0; i < fenSeperated[2].length(); i++){
        if (fenSeperated[2][i] == 'K') castling[0] = true;
        if (fenSeperated[2][i] == 'Q') castling[1] = true;
        if (fenSeperated[2][i] == 'k') castling[2] = true;
        if (fenSeperated[2][i] == 'q') castling[3] = true;
    }

    int enPSquare = -1;
    for (i = 0; i < fenSeperated[3].length(); i++) {
        // TODO need to add en passant square
    }
    enPassantSquare = enPSquare;

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
                outString.append(1, ' ');
                flag = 1;
            }
        }
        if (!flag) {
            outString.append(1, '-');
            outString.append(1, ' ');
        }
        counter += 1;
        if (counter == 8) {
            std::cout << outString << std::endl;
            counter = 0;
            outString = "";
        }
    }
    std::cout << "Turn: " << turn << " Attacking: " << attacking << std::endl;
}

// this needs to be sped up. Can do that with method template so each type of move is executed
void gameState::makeMove(const Move &move) {
    // done on a copy of the parent game state to create a child game state based on a move
    bitboards[move.piece].unSetBitAt(move.fromSquare);
    bitboards[move.piece].setBitAt(move.toSquare);

    if (move.captureFlag) {
        removePieceOnCapture(move.toSquare);
    }

    // white castling priv
    if (move.piece == ROOK) {
        if (move.fromSquare == h1) castling[0] = false;
        else if (move.fromSquare == a1) castling[1] = false;
    }
    // black castling priv
    if (move.piece == ROOK+6) {
        if (move.fromSquare == h8) castling[2] = false;
        else if (move.fromSquare == a8) castling[3] = false;
    }

    if (move.piece == K) castling[0] = false, castling[1] = false;
    if (move.piece == k) castling[2] = false, castling[3] = false;
    if (move.castleFlag) {
        // if it is black's turn
        if (turn) {
            // short castle
            if (move.toSquare == g8) {
                bitboards[ROOK + 6].unSetBitAt(h8);
                bitboards[ROOK + 6].setBitAt(f8);
            }
            else {
                bitboards[ROOK + 6].unSetBitAt(a8);
                bitboards[ROOK + 6].setBitAt(d8);
            }
            castling[2] = false;
            castling[3] = false;
        }
        else {
            // short castle
            if (move.toSquare == g1) {
                bitboards[ROOK].unSetBitAt(h1);
                bitboards[ROOK].setBitAt(f1);
            }
            else {
                bitboards[ROOK].unSetBitAt(a1);
                bitboards[ROOK].setBitAt(d1);
            }
            castling[0] = false;
            castling[1] = false;
        }
    }

    // just going to set en passant to the default unless a pawn is double pushed
    enPassantSquare = -1;
    if (move.doublePushFlag) {
        // will be -8 if it is white and +8 if it is black
        enPassantSquare = move.toSquare - 8 + (16*turn);
    }

    if (move.enPassantFlag){
        // if it is white's turn it will be -8 on the move to square whereas if it is black it will be a plus 8 thus 16 * 1 + 8
        removePieceOnCapture(move.toSquare - 8 + (16*turn));
    }

    if (move.promotedPiece) {
        bitboards[move.piece].unSetBitAt(move.toSquare);
        bitboards[move.promotedPiece].setBitAt(move.toSquare);
    }

    turn = ~turn;
    attacking = ~attacking;
}

// TODO clean up this function. It's gross
void gameState::stateToFen() {
    std::string output = "";
    BB allPcs = allPieces();
    int empty_squares = 0;
    int col_counter = 0; // for counting the number of columns
    int num_slashes = 0; // another dumb way of doing this but whatever
    BB location;
    for (int i = 63; i > -1; i --) {
        location = 1ULL << i;
        if (location & allPcs) {
            if (empty_squares) output += std::to_string(empty_squares);
            output += pieceToChar[getPieceAt(i)];
            empty_squares = 0;
            col_counter += 1;
        }
        else {
            empty_squares += 1;
            col_counter += 1;
        }
        if (col_counter == 8) {
            if (empty_squares) output += std::to_string(empty_squares);
            empty_squares = 0;
            col_counter = 0;
            num_slashes += 1;
            if (num_slashes < 8) output += '/';
        }
    }

    output += ' ';
    turn ? output += 'b' : output += 'w';
    output += ' ';

    // todo deal with no castling
    if (castling[0]) output += 'K';
    if (castling[1]) output += 'Q';
    if (castling[2]) output += 'k';
    if (castling[3]) output += 'q';
    output += ' ';
    (enPassantSquare != -1) ? output += boardMap[enPassantSquare] : output+= '-';
    output += " 0 1";

    std::cout << output << std::endl;
}

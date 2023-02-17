# chess_cpp
A chess engine written in C++ using bitboard based board representation.

### File Structure:
defsEnums.h: \
Imports definitions and dependencies which the rest of the program is reliant on.
Also contains the enums like Color, piece, etc. 

Bitboard.h/bitboard.cpp: \
A class for individual bitboards containing bitwise operations like setting and getting
bits and printing. Also contains methods to initialize and store leaping piece attack 
tables for fast move generation.

Magics.h/Magics.cpp: \
A class to initialize magic bitboards for rooks and bishops and store magic bitboards 
for move generation. The two main methods that interact with the rest of the game are 
getBishopAttacks and getRookAttacks which take the current game state and position to 
quickly return the attack bitboard for that piece.

Move.h: \
A class to represent an individual move. Contains all information necessary for a move: 
From square, to square, piece type, capture flag, double push flag, 
promoted piece, castle flag, and en passant flag.

GameState.h/GameState.cpp: \
A class for containing a board state. This class contains 12 bitboards, one for each type
of piece, the color of the current turn and the attacking pieces, as well as castling rights
and the square of a pawn which has double pushed allowing for an en passant. The methods in this 
class allow for additions of bitboards as well as making and un-making moves.

MoveGen.h/MoveGen.cpp: \
A class containing purely static methods for move generation. An instance of this class will 
not be made during the game as this is only for containing the methods necessary for move
generation.

Game.h/Game.cpp: \
The main game driver for operations relating to the actual gameplay of the chess game.

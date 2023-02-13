//
// Created by Thomas Mack on 04/02/2023.
//

#include "Testing.h"
using namespace std;

int Testing::AllTests() {
    string fenString = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -";

    /*gameState gs;
    gs.initialise(fenString);
    vector<Move> moves;
    moveGen::legalMoves(gs, moves);
    int depth = 1;
    BB total = 0ULL;

    for (auto move : moves) {
        gameState gsCopy = gs;
        gsCopy.makeMove(move);
        int temp = Perft(gsCopy, depth);
        cout << boardMap[move.fromSquare] << boardMap[move.toSquare] << " " << temp << "\n";
        total += temp;
    }

    cout << total;*/

    int perftDepth = 6;
    Testing::perftTest(fenString, perftDepth);

    return 0;
}

void Testing::perftTest(string fenString, int perftDepth) {
    gameState gs;
    gs.initialise(fenString);
    BB nodeCount;
    auto start = chrono::steady_clock::now();
    nodeCount = Perft(gs, perftDepth);
    //nodeCount = advancedPerftDriver(gs, perftDepth);
    auto end = chrono::steady_clock::now();
    auto diff = end - start;

    cout << nodeCount << " Time: " << chrono::duration <double, milli> (diff).count() << endl;
}

BB Testing::Perft(gameState &gs, int depth) {
    std::vector<Move> moves;
    moveGen::legalMoves(gs, moves);

    if (depth == 1) return moves.size();
    BB nodes = 0;
    for (Move move : moves) {
        gameState gsCopy = gs;
        gsCopy.makeMove(move);
        nodes += Perft(gsCopy, depth - 1);
    }
    return nodes;
}

BB Testing::PerftAdvanced(const gameState &gs, int depth, int *extraInfo) {
    // extra info contains information about ep, captures, castling, double push etc.
    // Order: Captures, E.P, Castle, Promotions
    std::vector<Move> moves;
    moveGen::legalMoves(gs, moves);

    if (depth == 1) {
        for (Move move : moves) {
            if (move.captureFlag) extraInfo[0] += 1;
            if (move.enPassantFlag) extraInfo[1] += 1;
            if (move.castleFlag) extraInfo[2] += 1;
            if (move.promotedPiece) extraInfo[3] += 1;
        }
        return moves.size();
    }
    BB nodes = 0;
    for (Move move : moves) {
        gameState gsCopy = gs;
        gsCopy.makeMove(move);
        nodes += PerftAdvanced(gsCopy, depth - 1, extraInfo);
    }
    return nodes;
}

BB Testing::advancedPerftDriver(gameState &gs, int depth) {
    int extraInfo[] = {0, 0, 0, 0};
    BB moveCount = 0;
    moveCount += PerftAdvanced(gs, depth, extraInfo);

    cout << "Move count: " << moveCount << " Captures: " << extraInfo[0] << " E.P. " << extraInfo[1] << " Castles: " << extraInfo[2] << " Promotions: " << extraInfo[3] << endl;

    return moveCount;
}
// Current Benchmarking
// 1041737766 Time: 478633 Perft: 6
// 3195018407 Time: 1.35963e+06 Perft: 7

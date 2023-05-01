//
// Created by Thomas Mack on 08/03/2023.
//

#ifndef TTCE_SEARCH_HPP
#define TTCE_SEARCH_HPP

#include "Evaluation.hpp"
#include "moveGen.hpp"
#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

namespace TtCE {

class Search {
private:
  int m_nodes;
  int m_maxDepth;
  int m_qNodes;
  int m_maxQDepth;
  int m_stopped;
  gameState m_gs;

  // timing stuff
  Move m_bestMove;
  long long m_startTime;
  long long m_endTime;
  Move m_hashBestMove;

  int pv_length[MAX_DEPTH];
  Move pv_table[MAX_DEPTH][MAX_DEPTH];

  // repetition
  BB repetition_table[1000];
  int repetition_index = 0;

  // killer moves

  static constexpr int FullDepthMoves = 4;
  static constexpr int ReductionLimit = 3;

public:
  Search() {
    m_nodes = 0;
    m_maxDepth = 0;
    m_qNodes = 0;
    m_maxQDepth = 4;
    BB m_currentPositionHash = 0;
    int stopped = 0;
    m_startTime = 0;
    m_endTime = 0;
    m_bestMove = Move{};
  }

  int getNodes() { return m_nodes; }
  int getQNodes() { return m_qNodes; }
  Move getBestMove() { return pv_table[0][0]; }

  // testing
  void setGs(const gameState &gs) { m_gs = gs; }
  gameState *getGs() { return &m_gs; }
  Move testingBestMove() { return m_bestMove; }

  void printBestMove() {
    std::cout << "bestmove ";
    pv_table[0][0].printMove();
    std::cout << "\n";
  }

  void unMakeMove(gameState oldGs) { m_gs = oldGs; }

  void setRepetitionMove(BB hash) {
    repetition_index++;
    repetition_table[repetition_index] = hash;
  }

  // resets all repetition table information
  // run before moves are initialised
  void resetRepetitionTable(BB hash) {
    repetition_index = 0;
    memset(repetition_table, 0ULL, sizeof(repetition_table));
    repetition_table[repetition_index] = hash;
  }

  static void initEngine() {
    Bitboard::initAttackTables();
    Magics::generateSlidingAttackTables();
    gameState::initZobrist();
  }

  void resetEngine() {
    // wipe old info
    memset(Move::killerMove, 0, sizeof(Move::killerMove));
    memset(Move::historyMove, 0, sizeof(Move::historyMove));
    clearPV();

    // init hash table
    gameState::clearTable();
  }

  void clearPV() {
    for (int i = 0; i < MAX_DEPTH; i++) {
      pv_length[i] = 0;
      for (int j = 0; j < MAX_DEPTH; j++) {
        pv_table[i][j] = Move{};
      }
    }
  }

  bool inline canMoveBeReduced(const Move &move, bool isInCheck) {
    return (!isInCheck && !move.m_captureFlag && !move.m_promotedPiece);
  }

  int quiescence(const gameState &gs, int alpha, int beta) {
    int ply = gs.getPly();
    m_qNodes++;

    if (m_qNodes % 2047 == 0)
      communicate();

    int eval = Eval::Evaluation::evaluate(gs);
    if (ply > 64)
      return eval;

    if (eval >= beta) {
      return beta;
    }

    if (eval > alpha) {
      alpha = eval;
    }

    std::vector<Move> moves;
    moveGen::legalMoves<Captures>(gs, moves);
    moveGen::sortMoves(gs, moves);

    for (Move move : moves) {
      gameState gsCopy = gs;
      gsCopy.makeMove(move, 0ULL);

      int score = -quiescence(gsCopy, -beta, -alpha);

      if (m_stopped)
        return 0;

      if (score > alpha) {
        alpha = score;

        if (score >= beta) {
          return beta;
        }
      }
    }
    return alpha;
  }

  void printPVLine() {
    for (int i = 0; i < m_maxDepth; i++) {
      if (pv_table[0][i].m_fromSquare == 0 && pv_table[0][i].m_toSquare == 0)
        continue;
      pv_table[0][i].printMove();
      std::cout << " ";
    }
  }

  int hashPVLine(BB hash, int depth) {
    gameState gs = m_gs;
    std::vector<Move> moves;
    moveGen::legalMoves<All>(gs, moves);

    if (!depth)
      return 0;

    int bestScore = 0;
    Move bestMove{};

    for (Move move : moves) {
      BB newHash = gs.makeMove(move, hash);
      gameState::TT *hashEntry = gameState::getHashForPV(newHash);
      int score = hashEntry->score;

      if (score > bestScore) {
        bestScore = score;
        bestMove = move;
      }
    }

    bestMove.printMove();
    return 0;
  }

  int findBestMove(const gameState &gs, int depth, int usingTime) {
    m_stopped = 0;
    if (usingTime) {
      m_startTime = time_ms();
      m_endTime = m_startTime + usingTime;
    }

    int score;
    int alpha;
    int beta;
    int currentDepth;
    alpha = -INFINITE;
    beta = INFINITE;
    m_maxDepth = 0;

    // iterative deepening
    for (m_maxDepth = 1; m_maxDepth <= depth; m_maxDepth++) {
      m_gs = gs;
      m_gs.setPly(0);
      auto begin = std::chrono::high_resolution_clock::now();
      m_nodes = 0;
      m_qNodes = 0;
      currentDepth = m_maxDepth;
      BB posHash = m_gs.getHash();
      score = negamax(alpha, beta, currentDepth, posHash, true);
      auto end = std::chrono::high_resolution_clock::now();
      auto duration =
          std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin)
              .count();

      float nps = 0.0;
      if (duration) {
        nps = (m_nodes + m_qNodes) * 1000000000.0 / duration;
      } else {
        nps = 1.0;
      }
      // engine info section
      if (m_stopped) {
        std::cout << "Stopped after: " << time_ms() - m_startTime << "\n";
        break;
      } else {
        std::cout << "info depth " << currentDepth;
        if (score > -MATE_VALUE && score < -MATE_SCORE) {
          std::cout << " mate " << -(score + MATE_VALUE) / 2 - 1;

        } else if (score < MATE_VALUE && score > MATE_SCORE) {
          std::cout << " mate " << (MATE_VALUE - score) / 2 - 1;

        } else {
          std::cout << " score cp " << score;
        }
        std::cout << " time " << duration / 1000000 << " nodes "
                  << m_nodes + m_qNodes << " nps " << nps << " pv ";

        printPVLine();
        std::cout << std::endl;
        m_bestMove = pv_table[0][0];
      }

      // aspiration window
      // TODO: Testing with and without by having engine play itself
      if ((score <= alpha) || (score >= beta)) {
        alpha = -INFINITE;
        beta = INFINITE;
        continue;
      }
      alpha = score - 50;
      beta = score + 50;
    }

    if (!m_stopped) {
      std::cout << "bestmove ";
      pv_table[0][0].printMove();
      std::cout << "\n";
    } else {
      std::cout << "bestmove ";
      m_bestMove.printMove();
      std::cout << "\n";
    }
    return score;
  }

  bool inline is_repetition(BB hash) {
    // TODO: repetition table should only look a few moves back
    for (int i = 0; i < repetition_index; i++) {
      if (repetition_table[i] == hash) {
        return true;
      }
    }
    return false;
  }

  void communicate() {
    if (m_startTime && (time_ms() > m_endTime)) {
      m_stopped = 1;
    }
    // TODO: read user input to see if stop or quit
  }

  // this function is thanks to CMK on youtube:
  // https://www.youtube.com/@chessprogramming591
  int negamax(int alpha, int beta, int depth, BB hash, bool PVLine) {
    int score;
    int ply = m_gs.getPly();
    int hashFlag = hashFlagExact;

    if (ply && is_repetition(hash))
      return 0;

    int pv_node = beta - alpha > 1;

    if (ply &&
        (score = gameState::getHashEntry(hash, alpha, beta, depth)) !=
            valUnknown &&
        pv_node == 0)
      return score;

    // every 2047 nodes
    if ((m_nodes & 2047) == 0)
      // "listen" to the GUI/user input
      communicate();

    // init PV length
    pv_length[ply] = ply;

    if (depth == 0)
      return quiescence(m_gs, alpha, beta);

    m_nodes++;

    int in_check = m_gs.isKingInCheck();

    if (in_check)
      depth++;

    // null move pruning
    if (depth >= 3 && in_check == 0 && ply) {
      gameState gsCopy = m_gs;
      BB newHash = m_gs.makeNullMove(hash);

      repetition_index++;
      repetition_table[repetition_index] = newHash;

      score = -negamax(-beta, -beta + 1, depth - 1 - 2, newHash, false);

      repetition_index--;
      m_gs = gsCopy;
      // reutrn 0 if time is up
      if (m_stopped == 1)
        return 0;

      // fail-hard beta cutoff
      if (score >= beta)
        // node (position) fails high
        return beta;
    }

    std::vector<Move> moves;
    moveGen::legalMoves<All>(m_gs, moves);

    bool continuePV = false;
    if (PVLine) {
      // moveGen::sortMoves(m_gs, moves);
      Move pvMove = pv_table[0][ply];
      continuePV = moveGen::sortMovesPV(m_gs, moves, pvMove);
    } else {
      moveGen::sortMoves(m_gs, moves);
    }

    int searchedMoves = 0;

    for (Move move : moves) {
      gameState gsCopy = m_gs;

      BB newHash = m_gs.makeMove(move, hash);

      repetition_index++;
      repetition_table[repetition_index] = newHash;

      if (searchedMoves == 0)
        // normal search on pv node/best move
        score = -negamax(-beta, -alpha, depth - 1, newHash, continuePV);

      else {
        // LMR
        if (searchedMoves >= FullDepthMoves && depth >= ReductionLimit &&
            in_check == 0 && !move.m_captureFlag && !move.m_promotedPiece)
          score = -negamax(-alpha - 1, -alpha, depth - 2, newHash, false);

        else
          // make sure the moves that pass by LMR are still searched
          score = alpha + 1;

        // PVS
        if (score > alpha) {
          score = -negamax(-alpha - 1, -alpha, depth - 1, newHash, false);

          if ((score > alpha) && (score < beta))
            score = -negamax(-beta, -alpha, depth - 1, newHash, false);
        }
      }

      // decrement repetition index
      repetition_index--;
      // take back move
      m_gs = gsCopy;

      // reutrn 0 if time is up
      if (m_stopped)
        return 0;

      searchedMoves++;

      if (score > alpha) {
        hashFlag = hashFlagExact;

        if (!move.m_captureFlag)
          Move::historyMove[move.m_piece][move.m_toSquare] += depth;

        alpha = score;
        // PV table
        pv_table[ply][ply] = move;
        for (int next_ply = ply + 1; next_ply < pv_length[ply + 1]; next_ply++)
          pv_table[ply][next_ply] = pv_table[ply + 1][next_ply];
        pv_length[ply] = pv_length[ply + 1];

        if (score >= beta) {
          gameState::insertHash(hash, beta, depth, hashFlagBeta, ply);

          if (!move.m_captureFlag) {
            Move::killerMove[1][ply] = Move::killerMove[0][ply];
            Move::killerMove[0][ply] = move;
          }
          return beta;
        }
      }
    }

    if (!moves.size()) {
      if (in_check)
        return -MATE_VALUE + ply;
      else
        return 0;
    }
    gameState::insertHash(hash, alpha, depth, hashFlag, ply);

    return alpha;
  }
};
} // namespace TtCE

#endif // TTCE_SEARCH_HPP

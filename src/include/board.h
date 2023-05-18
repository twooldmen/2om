#ifndef BOARD_H
#define BOARD_H

/*
 * Interface for an optimized version of the Two Old Men game board.
 * Includes all the functions required for basic gameplay
 */

#include <cstdint>
#include <iostream>
using std::cin, std::cout;

#include <math.h>
#include <string>
using std::string, std::to_string;

// for memcpy
#include <cstring>

#include <vector>
using std::vector;

#include <queue>
using std::queue;

#include "constants.h"

#define Action uint8_t

class BitBoard
{

public:
    BitBoard();
    BitBoard(const BitBoard &toCopy);
    BitBoard &operator=(const BitBoard &toCopy);

    vector<char> getGlacier() const;
    int getWhitePlayer() const;
    int getBlackPlayer() const;
    int getWhiteIce() const;
    int getBlackIce() const;
    int getWinner() const;
    int getTurn() const;
    int getTurns() const;
    string getStatement() const;
    Action getLastMove() const;

    vector<bool> checkDirs();
    void setPlayerMoves(int moveType);
    void play(int moveType, int dir);
    void playAction(Action move);
    vector<Action> getValidActions();

    int eval();
    // float reward();
    // int hash();

    void debug();

private:

    struct Board
    {
        uint64_t top;
        uint64_t bot;

        Board operator~()
        {
            Board result = *this;
            result.top = ~result.top;
            result.bot = ~result.bot;

            return result;
        }

        Board operator^(const Board &b)
        {
            Board result = *this;
            result.top ^= b.top;
            result.bot ^= b.bot;

            return result;
        }

        Board operator&(const Board &b)
        {
            Board result = *this;
            result.top &= b.top;
            result.bot &= b.bot;

            return result;
        }

        Board operator|(const Board &b)
        {
            Board result = *this;
            result.top |= b.top;
            result.bot |= b.bot;

            return result;
        }

        Board operator^=(uint64_t n)
        {
            top ^= n;
            bot ^= n;

            return *this;
        }

        Board operator^=(const Board &b)
        {
            top ^= b.top;
            bot ^= b.bot;

            return *this;
        }

        Board operator&=(const Board &b)
        {
            top &= b.top;
            bot &= b.bot;

            return *this;
        }

        Board operator|=(const Board &b)
        {
            top |= b.top;
            bot |= b.bot;

            return *this;
        }

        operator bool()
        {
            return top || bot;
        }
    };

    struct Player
    {
        Board board;
        Board glacier;
        int ice;
        int volume;
    };

    Board board;

    char glacierTop[64];
    char glacierBot[64];

    Player players[2];
    Board validMoves;
    bool validDirs[6];
    int turn;
    int turns;
    int winner;
    string statement;
    Action lastMove;

    void init();

    int getBitCount(uint64_t n) const;
    int bitToIdx(uint64_t n) const;

    void printBoard();
    void printGlacier();
    void printNbrs(Board nbrs);

    void changeTurn();

    Board getNeighbors(Board loc);
    Board getUp(Board loc);
    Board getUpRight(Board loc);
    Board getRight(Board loc);
    Board getDown(Board loc);
    Board getDownLeft(Board loc);
    Board getLeft(Board loc);

    void setValidMoves(int moveType, Board loc);
    void bfs(Board loc, Board &vis);
    void findRemaining();
    void take(int dir);
    void place(int dir);
    void move(int dir);

    void conBFS(Board loc, Board &vis, Board nbrs);
};

enum Moves
{
    TAKE,
    PLACE,
    MOVE
};

enum Directions
{
    UP,             // 6
    UP_RIGHT,       // 1
    RIGHT,          // 2
    DOWN,           // 3
    DOWN_LEFT,      // 4
    LEFT            // 5
};

#endif

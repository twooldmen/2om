#ifndef BOT_H
#define BOT_H

#include <chrono>
#include <map>
using std::map;

#include <utility>
using std::pair;

#include <fstream>
#include <iostream>
using std::ostream;
using std::ofstream;

#include <algorithm>
using std::sort;

#include "constants.h"
#include "board.h"

struct Search
{
    int score;
    BitBoard state;
    Action move;
};

string moveToString(Action move);
Search negaScout(BitBoard &board, int depth, int alpha, int beta, int turn);
void engine(BitBoard &board);

#endif

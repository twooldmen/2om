#include "include/bot.h"

clock_t START_TIME;
double LIMIT = 3.0;
int nodes = 0;

int pvLength[MAX_DEPTH];
Action pvTable[MAX_DEPTH][MAX_DEPTH];
int ply = 0;

/*
 * moveToString
 * Return the string representation of a given action
 */
string moveToString(Action move)
{
    string s = "";
    move & 0b01000000 ? s += "P" : s += "T";
    s += to_string(((move & 0b00111000) >> 3));
    s += "m";
    s += to_string((move & 0b00000111));

    if (s[1] == '0')
    {
        s[1] = '6';
    }
    if (s[3] == '0')
    {
        s[3] = '6';
    }

    return s;
}

/*
 * timeUp
 * Return true if the time taken for the bot has exceeded the specified limit
 */
bool timeUp()
{
    return ((float)(clock() - START_TIME) / (float)(CLOCKS_PER_SEC)) >= LIMIT;
}

/*
 * negaScout
 * Perform Principled Variation Search
 */
Search negaScout(BitBoard &board, int depth, int alpha, int beta, int turn)
{
    pvLength[ply] = ply;
    nodes++;

    if (depth == 0 || board.getWinner() > -1)
    {
        return { turn == 0 ? board.eval() : -board.eval(), board, board.getLastMove() };
    }

    BitBoard alphaState;
    Action alphaMove;
    vector<Action> moves = board.getValidActions();

    // stalemate
    if (moves.size() == 0)
    {
        BitBoard stalemate = board;
        stalemate.setPlayerMoves(TAKE);
        vector<bool> v = stalemate.checkDirs();

        for (int i = 0; i < v.size(); i++)
        {
            if (v[i])
            {
                stalemate.play(TAKE, i);
                Action a = 0b10000000;
                a |= (i << 3);

                pvTable[ply][ply] = a;
                return { -1000 + stalemate.getTurns(), stalemate, stalemate.getLastMove() };
            }
        }
    }

    // if we only have 1 move to make then make it automatically
    if (moves.size() == 1)
    {
        BitBoard state = board;
        state.playAction(moves[0]);

        pvTable[ply][ply] = moves[0];
        return { turn == 0 ? state.eval() : -state.eval(), state, moves[0] };
    }

    // find winning move first if possible
    for (int i = 0; i < moves.size(); i++)
    {
        // only take moves are winning, break when we hit a place move
        if (moves[i] & 0b01000000)
        {
            break;
        }

        BitBoard winning = board;
        winning.playAction(moves[i]);

        if (winning.getWinner() == turn)
        {
            pvTable[ply][ply] = moves[i];
            return { turn == 0 ? winning.eval() : -winning.eval(), winning, moves[i] };
        }
    }

    for (int i = 0; i < moves.size(); i++)
    {
        int score = 0;
        BitBoard state = board;
        state.playAction(moves[i]);

        ply++;

        // principled variation search
        if (i == 0)
        {
            score = -negaScout(state, depth - 1, -beta, -alpha, turn ^ 1).score;
        }

        else
        {
            // null window search
            score = -negaScout(state, depth - 1, -alpha - 1, -alpha, turn ^ 1).score;

            // new search on fail-high
            if (alpha < score && beta > score)
            {
                score = -negaScout(state, depth - 1, -beta, -score, turn ^ 1).score;
            }
        }

        ply--;

        if (alpha < score)
        {
            alpha = score;
            alphaState = state;
            alphaMove = moves[i];

            pvTable[ply][ply] = moves[i];
            std::memcpy(&pvTable[ply][ply + 1], &pvTable[ply + 1][ply + 1], sizeof(Action) * (pvLength[ply + 1] - ply - 1));
            pvLength[ply] = pvLength[ply + 1];
        }

        if (alpha >= beta)
        {
            break;
        }
    }

    return { alpha, alphaState, alphaMove };
}

/*
 * engine
 * Perform iterative deepening of the PVS/NegaScout algorithm
 * Print the optimal line to the console and the time taken for computations
 */
void engine(BitBoard &board)
{
    for (int i = 0; i < MAX_DEPTH; i++)
    {
        pvLength[i] = 0;
        std::memset(pvTable[i], (Action)(0), sizeof(Action) * MAX_DEPTH);
    }

    // START_TIME = clock();
    Search bestSearch;
    int depth = 6;
    nodes = 0;

    cout << "Thinking...\n";
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 1; i <= depth; i++)
    {
        Search search = negaScout(board, i, -1e9, 1e9, board.getTurn());
        bestSearch = search;
    }

    auto stop = std::chrono::high_resolution_clock::now();

    cout << "Depth: " << depth << "\n";
    cout << "Best move: " << moveToString(bestSearch.move) << "\n";
    cout << "Line: ";
    for (int i = 0; i < depth; i++)
    {
        cout << moveToString(pvTable[0][i]) << " ";
    }
    cout << "\n";

    cout << "Score: " << bestSearch.score << "\n";
    cout << "Nodes searched: " << nodes << "\n";

    auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    cout << "Time elapsed: " << time.count() / 1e6 << "\n\n";
    board = bestSearch.state;
}

#include "include/board.h"

/*
 * Default constructor
 */
BitBoard::BitBoard()
{
    init();
    // printBoard();
}

/*
 * Copy constructor
 */
BitBoard::BitBoard(const BitBoard &toCopy)
{
    board = toCopy.board;
    validMoves = toCopy.validMoves;
    turn = toCopy.turn;
    turns = toCopy.turns;
    winner = toCopy.winner;
    statement = toCopy.statement;
    lastMove = toCopy.lastMove;

    players[0] = toCopy.players[0];
    players[1] = toCopy.players[1];

    memcpy(glacierTop, toCopy.glacierTop, 64 * sizeof(char));
    memcpy(glacierBot, toCopy.glacierBot, 64 * sizeof(char));
    memcpy(validDirs, toCopy.validDirs, 6 * sizeof(bool));
}

/*
 * Assignment operator
 */
BitBoard &BitBoard::operator=(const BitBoard &toCopy)
{
    if (this != &toCopy)
    {
        board = toCopy.board;
        validMoves = toCopy.validMoves;
        turn = toCopy.turn;
        turns = toCopy.turns;
        winner = toCopy.winner;
        statement = toCopy.statement;
        lastMove = toCopy.lastMove;

        players[0] = toCopy.players[0];
        players[1] = toCopy.players[1];

        memcpy(glacierTop, toCopy.glacierTop, 64 * sizeof(char));
        memcpy(glacierBot, toCopy.glacierBot, 64 * sizeof(char));
        memcpy(validDirs, toCopy.validDirs, 6 * sizeof(bool));
    }

    return *this;
}

/*
 * init
 * Resets all parameters of the game board as if a new game is starting
 * from scratch
 */
void BitBoard::init()
{
    /*
     * 0001111001111101111111111111111111011111001111000 (49)
     *
     * 0001111
     * 0011111
     * 0111111
     * 1111111
     * 1111110
     * 1111100
     * 1111000
     */

    /*
     * 121
     *
     * 00000000000
     * 00000000000
     * 00000111100
     * 00001111100
     * 00011111100
     * 001111
     *       11100
     * 00111111000
     * 00111110000
     * 00111100000
     * 00000000000
     * 00000000000
     */

    uint64_t base      = 0b0001111001111101111111111111111111011111001111000;
    board.top = 0b0000000000000000000000000001111000000111110000011111100001111;
    board.bot = 0b0000111000011111100000111110000001111000000000000000000000000000;

    memset(glacierTop, '0', 64 * sizeof(char));
    memset(glacierBot, '0', 64 * sizeof(char));

    memset(glacierTop + 0, '1', 4 * sizeof(char));
    memset(glacierTop + 8, '1', 6 * sizeof(char));
    memset(glacierTop + 19, '1', 5 * sizeof(char));

    memset(glacierTop + 30, '1', 4 * sizeof(char));
    memset(glacierBot + 57, '1', 3 * sizeof(char));

    memset(glacierBot + 47, '1', 6 * sizeof(char));
    memset(glacierBot + 37, '1', 5 * sizeof(char));
    memset(glacierBot + 27, '1', 4 * sizeof(char));

    /*
     *
     * 00000000000
     * 00000000000
     * 00000111100
     * 00001011100
     * 00011111100
     * 001111
     *       11100
     * 00111111000
     * 00111010000
     * 00111100000
     * 00000000000
     * 00000000000
     */
    players[0] = { { 0, 1ULL << 38 }, board, 0 };
    players[1] = { { 1ULL << 22, 0 }, board, 0 };

    memset(validDirs, false, 6 * sizeof(bool));

    // players[0] = { 0, 1ULL << 38, 0 };
    // players[1] = { 1ULL << 22, 0, 0 };

    turn = 0;
    turns = 0;
    winner = -1;
    statement = "White: Take/Place";
    lastMove = 0b10000000;
}

/*
 * getBitCount
 * Returns the number of set bits in a 64 bit integer
 */
int BitBoard::getBitCount(uint64_t n) const
{
    int count = 0;
    while (n)
    {
        n &= (n - 1);
        count++;
    }

    return count;
}

/*
 * bitToIdx
 * Returns the index of the largest bit set in a 64 bit integer
 */
int BitBoard::bitToIdx(uint64_t n) const
{
    return 63 - __builtin_clzll(n);
}

/*
 * printBoard
 * Prints a readable representation of the game board
 */
void BitBoard::printBoard()
{
    int line = 0;
    for (int i = 61; i > 0; i--)
    {

        cout << ((board.top & (1LL << (i - 1))) != 0) << " ";

        line++;
        if (line == NUM)
        {
            cout << "\n";
            line = 0;
        }
    }

    for (int i = 60; i > 0; i--)
    {
        cout << ((board.bot & (1LL << (i - 1))) != 0) << " ";

        line++;
        if (line == NUM)
        {
            cout << "\n";
            line = 0;
        }
    }

    cout << "\n\n";
}

/*
 * printGlacier
 * Prints a readable representation of the glacier
 */
void BitBoard::printGlacier()
{
    cout << "W: " << players[0].ice << "\t\t";
    cout << "B: " << players[1].ice;
    cout << "\n";
    int line = 0;

    for (int i = 61; i > 0; i--)
    {
        if (players[0].board.top & (1ULL << (i - 1)))
        {
            cout << "W ";
        }

        else if (players[1].board.top & (1ULL << (i - 1)))
        {
            cout << "B ";
        }

        else
        {
            cout << glacierTop[i - 1] << " ";
        }
        line++;

        if (line == NUM)
        {
            cout << "\n";
            line = 0;
        }
    }

    for (int i = 60; i > 0; i--)
    {
        if (players[0].board.bot & (1ULL << (i - 1)))
        {
            cout << "W ";
        }

        else if (players[1].board.bot & (1ULL << (i - 1)))
        {
            cout << "B ";
        }

        else
        {
            cout << glacierBot[i - 1] << " ";
        }
        line++;

        if (line == NUM)
        {
            cout << "\n";
            line = 0;
        }
    }

    cout << "\n\n";
}

/*
 * printNbrs
 * Prints a readable representation of an inputted board
 */
void BitBoard::printNbrs(Board nbrs)
{
    int line = 0;
    for (int i = 61; i > 0; i--)
    {

        cout << ((nbrs.top & (1LL << (i - 1))) != 0) << " ";

        line++;
        if (line == NUM)
        {
            cout << "\n";
            line = 0;
        }
    }

    for (int i = 60; i > 0; i--)
    {
        cout << ((nbrs.bot & (1LL << (i - 1))) != 0) << " ";

        line++;
        if (line == NUM)
        {
            cout << "\n";
            line = 0;
        }
    }

    cout << "\n\n";
}

/*
 * debug
 * Prints useful information regarding the game state for debugging purposes
 */
void BitBoard::debug()
{
    printGlacier();
    printBoard();
    printNbrs(validMoves);
    cout << "Valid Dirs: ";
    for (auto x : validDirs)
    {
        cout << x << " ";
    }
    cout << "\n\n";
}

/*
 * changeTurn
 * Change the current player's turn and increment the number of turns taken
 */
void BitBoard::changeTurn()
{
    turn ^= 1;
    turns++;
}

/*
 * getNeighbors
 * Returns a board containing all neighbors of set bits in a given board
 */
BitBoard::Board BitBoard::getNeighbors(Board loc)
{
    Board nbrs = { 0, 0 };

    nbrs |= getUp(loc);
    nbrs |= getUpRight(loc);
    nbrs |= getRight(loc);
    nbrs |= getDown(loc);
    nbrs |= getDownLeft(loc);
    nbrs |= getLeft(loc);

    return nbrs;
}

/*
 * getUp
 * Returns a board containing the set bits one above the set bits in a given board
 */
BitBoard::Board BitBoard::getUp(Board loc)
{
    Board nbrs = { 0, 0 };

    if (loc.bot != 0 && (loc.bot & (0b11111111111ULL << 49)))
    {
        uint64_t bitsToShift = (loc.bot & (0b11111111111ULL << 49));
        bitsToShift >>= 25;
        uint64_t x = 0;

        x |= (bitsToShift << NUM);
        x >>= 35;

        nbrs.top |= x;
    }

    if (loc.top != 0 && (loc.top & (0b11111111111ULL)))
    {
        uint64_t bitsToShift = (loc.top & (0b11111111111ULL));
        bitsToShift <<= 25;
        uint64_t x = 0;

        x |= (bitsToShift << NUM);
        x <<= 35;

        nbrs.bot |= x;
    }

    nbrs.top |= (loc.top << NUM);
    nbrs.bot |= (loc.bot << NUM);

    nbrs.bot &= ~(0b11111111111ULL);

    return nbrs;
}

/*
 * getDown
 * Returns a board containing the set bits one below the set bits in a given board
 */
BitBoard::Board BitBoard::getDown(Board loc)
{
    Board nbrs = { 0, 0 };

    if (loc.bot != 0 && (loc.bot & (0b11111111111ULL << 49)))
    {
        uint64_t bitsToShift = (loc.bot & (0b11111111111ULL << 49));
        bitsToShift >>= 25;
        uint64_t x = 0;

        x |= (bitsToShift >> NUM);
        x >>= 35;

        nbrs.top |= x;
    }

    if (loc.top != 0 && (loc.top & (0b11111111111ULL)))
    {
        uint64_t bitsToShift = (loc.top & (0b11111111111ULL));
        bitsToShift <<= 25;
        uint64_t x = 0;

        x |= (bitsToShift >> NUM);
        x <<= 35;

        nbrs.bot |= x;
    }

    nbrs.top |= (loc.top >> NUM);
    nbrs.bot |= (loc.bot >> NUM);

    nbrs.top &= ~(0b11111111111ULL << 50);

    return nbrs;
}

/*
 * getUpRight
 * Returns a board containing the set bits one above and to the right of the set bits in a given board
 */
BitBoard::Board BitBoard::getUpRight(Board loc)
{
    Board nbrs = { 0, 0 };

    if (loc.bot != 0 && (loc.bot & (0b11111111111ULL << 49)))
    {
        uint64_t bitsToShift = (loc.bot & (0b11111111111ULL << 49));
        bitsToShift >>= 25;
        uint64_t x = 0;

        x |= (bitsToShift << (NUM - 1));
        x >>= 35;

        nbrs.top |= x;
    }

    if (loc.top != 0 && (loc.top & (0b11111111111ULL)))
    {
        uint64_t bitsToShift = (loc.top & (0b11111111111ULL));
        bitsToShift <<= 25;
        uint64_t x = 0;

        x |= (bitsToShift << (NUM - 1));
        x <<= 35;

        nbrs.bot |= x;
    }

    nbrs.top |= (loc.top << (NUM - 1));
    nbrs.bot |= (loc.bot << (NUM - 1));

    nbrs.bot &= ~(0b11111111111ULL);

    nbrs.top &= ~(1ULL << 5);
    nbrs.top &= ~(1ULL << 16);
    nbrs.top &= ~(1ULL << 27);
    nbrs.top &= ~(1ULL << 38);
    nbrs.top &= ~(1ULL << 49);

    nbrs.bot &= ~(1ULL << 10);
    nbrs.bot &= ~(1ULL << 21);
    nbrs.bot &= ~(1ULL << 32);
    nbrs.bot &= ~(1ULL << 43);
    nbrs.bot &= ~(1ULL << 54);

    return nbrs;
}

/*
 * getDownLeft
 * Returns a board containing the set bits one below and to the left of the set bits in a given board
 */
BitBoard::Board BitBoard::getDownLeft(Board loc)
{
    Board nbrs = { 0, 0 };

    if (loc.bot != 0 && (loc.bot & (0b11111111111ULL << 49)))
    {
        uint64_t bitsToShift = (loc.bot & (0b11111111111ULL << 49));
        bitsToShift >>= 25;
        uint64_t x = 0;

        x |= (bitsToShift >> (NUM - 1));
        x >>= 35;

        nbrs.top |= x;
    }

    if (loc.top != 0 && (loc.top & (0b11111111111ULL)))
    {
        uint64_t bitsToShift = (loc.top & (0b11111111111ULL));
        bitsToShift <<= 25;
        uint64_t x = 0;

        x |= (bitsToShift >> (NUM - 1));
        x <<= 35;

        nbrs.bot |= x;
    }

    nbrs.top |= (loc.top >> (NUM - 1));
    nbrs.bot |= (loc.bot >> (NUM - 1));

    nbrs.top &= ~(0b11111111111ULL << 50);

    nbrs.top &= ~(1ULL << 6);
    nbrs.top &= ~(1ULL << 17);
    nbrs.top &= ~(1ULL << 28);
    nbrs.top &= ~(1ULL << 39);
    nbrs.top &= ~(1ULL << 50);

    nbrs.bot &= ~(1ULL << 0);
    nbrs.bot &= ~(1ULL << 11);
    nbrs.bot &= ~(1ULL << 22);
    nbrs.bot &= ~(1ULL << 33);
    nbrs.bot &= ~(1ULL << 44);
    nbrs.bot &= ~(1ULL << 55);

    return nbrs;
}

/*
 * getRight
 * Returns a board containing the set bits one to the right of the set bits in a given board
 */
BitBoard::Board BitBoard::getRight(Board loc)
{
    Board nbrs = { 0, 0 };

    if (loc.bot != 0 && (loc.bot & (0b11111111111ULL << 49)))
    {
        uint64_t bitsToShift = (loc.bot & (0b11111111111ULL << 49));
        bitsToShift >>= 25;
        uint64_t x = 0;

        x |= (bitsToShift >> 1);
        x >>= 35;

        nbrs.top |= x;
    }

    if (loc.top != 0 && (loc.top & (0b11111111111ULL)))
    {
        uint64_t bitsToShift = (loc.top & (0b11111111111ULL));
        bitsToShift <<= 25;
        uint64_t x = 0;

        x |= (bitsToShift >> 1);
        x <<= 35;

        nbrs.bot |= x;
    }

    nbrs.top |= (loc.top >> 1);
    nbrs.bot |= (loc.bot >> 1);

    nbrs.top &= ~(1ULL << 5);
    nbrs.top &= ~(1ULL << 16);
    nbrs.top &= ~(1ULL << 27);
    nbrs.top &= ~(1ULL << 38);
    nbrs.top &= ~(1ULL << 49);

    nbrs.bot &= ~(1ULL << 10);
    nbrs.bot &= ~(1ULL << 21);
    nbrs.bot &= ~(1ULL << 32);
    nbrs.bot &= ~(1ULL << 43);
    nbrs.bot &= ~(1ULL << 54);

    return nbrs;
}

/*
 * getLeft
 * Returns a board containing the set bits one to the left of the set bits in a given board
 */
BitBoard::Board BitBoard::getLeft(Board loc)
{
    Board nbrs = { 0, 0 };

    if (loc.bot != 0 && (loc.bot & (0b11111111111ULL << 49)))
    {
        uint64_t bitsToShift = (loc.bot & (0b11111111111ULL << 49));
        bitsToShift >>= 25;
        uint64_t x = 0;

        x |= (bitsToShift << 1);
        x >>= 35;

        nbrs.top |= x;
    }

    if (loc.top != 0 && (loc.top & (0b11111111111ULL)))
    {
        uint64_t bitsToShift = (loc.top & (0b11111111111ULL));
        bitsToShift <<= 25;
        uint64_t x = 0;

        x |= (bitsToShift << 1);
        x <<= 35;

        nbrs.bot |= x;
    }

    nbrs.top |= (loc.top << 1);
    nbrs.bot |= (loc.bot << 1);

    nbrs.top &= ~(1ULL << 6);
    nbrs.top &= ~(1ULL << 17);
    nbrs.top &= ~(1ULL << 28);
    nbrs.top &= ~(1ULL << 39);
    nbrs.top &= ~(1ULL << 50);

    nbrs.bot &= ~(1ULL << 0);
    nbrs.bot &= ~(1ULL << 11);
    nbrs.bot &= ~(1ULL << 22);
    nbrs.bot &= ~(1ULL << 33);
    nbrs.bot &= ~(1ULL << 44);
    nbrs.bot &= ~(1ULL << 55);

    return nbrs;
}

/*
 * checkDirs
 * Returns a vector containing all the valid directions the player can perform an action in
 */
vector<bool> BitBoard::checkDirs()
{
    memset(validDirs, false, 6 * sizeof(bool));
    Board loc = turn == 0 ? players[0].board : players[1].board;

    if (validMoves & getUp(loc))
    {
        validDirs[UP] = true;
    }

    if (validMoves & getUpRight(loc))
    {
        validDirs[UP_RIGHT] = true;
    }

    if (validMoves & getRight(loc))
    {
        validDirs[RIGHT] = true;
    }

    if (validMoves & getDown(loc))
    {
        validDirs[DOWN] = true;
    }

    if (validMoves & getDownLeft(loc))
    {
        validDirs[DOWN_LEFT] = true;
    }

    if (validMoves & getLeft(loc))
    {
        validDirs[LEFT] = true;
    }

    vector<bool> v(6);
    std::copy(validDirs, validDirs + 6, v.begin());

    return v;
}

/*
 * setValidMoves
 * Sets the validMoves board to contain set bits indicating the valid locations to perform an action from a given location
 */
void BitBoard::setValidMoves(int moveType, Board loc)
{
    validMoves = { 0, 0 };
    Board nbrs = getNeighbors(loc);

    switch (moveType)
    {
        case TAKE:
        {
            validMoves = board & nbrs & ~players[turn ^ 1].board;
            break;
        }

        case PLACE:
        {
            if (players[turn].ice)
            {
                Board doubleNbrs = getNeighbors(nbrs) & board;
                Board placeEmpty = nbrs & ~players[turn ^ 1].board;
                validMoves = board & placeEmpty;
                placeEmpty ^= validMoves;

                while (placeEmpty)
                {
                    Board b = { 0, 0 };
                    if (placeEmpty.top)
                    {
                        int idx = bitToIdx(placeEmpty.top);
                        b = { 1ULL << idx, 0 };

                        Board x = getNeighbors(b) & doubleNbrs;

                        if (getBitCount(x.top) + getBitCount(x.bot) > 1)
                        {
                            validMoves.top |= b.top;
                        }
                    }

                    if (placeEmpty.bot)
                    {
                        int idx = bitToIdx(placeEmpty.bot);
                        b = { 0, 1ULL << idx };

                        Board x = getNeighbors(b) & doubleNbrs;

                        if (getBitCount(x.top) + getBitCount(x.bot) > 1)
                        {
                            validMoves.bot |= b.bot;
                        }
                    }

                    placeEmpty &= ~b;
                }
            }

            break;
        }

        case MOVE:
        {
            validMoves = board & nbrs & ~getNeighbors(players[turn ^ 1].board);

            if (validMoves == 0)
            {
                winner = turn ^ 1;
                statement = winner == 0 ? "White wins by [stalemate]" : "Black wins by [stalemate]";
                break;
            }

            int currIdx = loc.top ? bitToIdx(loc.top) : bitToIdx(loc.bot);
            Board tmp = validMoves;
            while (tmp)
            {
                Board b = { 0, 0 };
                if (tmp.top)
                {
                    int idx = bitToIdx(tmp.top);
                    b = { 1ULL << idx, 0 };

                    if (loc.top)
                    {
                        if (glacierTop[idx] - glacierTop[currIdx] > 1)
                        {
                            validMoves &= ~b;
                        }
                    }

                    if (loc.bot)
                    {
                        if (glacierTop[idx] - glacierBot[currIdx] > 1)
                        {
                            validMoves &= ~b;
                        }
                    }
                }

                if (tmp.bot)
                {
                    int idx = bitToIdx(tmp.bot);
                    b = { 0, 1ULL << idx };

                    if (loc.top)
                    {
                        if (glacierBot[idx] - glacierTop[currIdx] > 1)
                        {
                            validMoves &= ~b;
                        }
                    }

                    if (loc.bot)
                    {
                        if (glacierBot[idx] - glacierBot[currIdx] > 1)
                        {
                            validMoves &= ~b;
                        }
                    }
                }

                tmp ^= b;
            }

            break;
        }

        default:
        {
            break;
        }
    }

    validMoves.bot &= ~(0b1111ULL << 60);
}

/*
 * setPlayerMoves
 * Sets the validMoves board to contain set bits indicating the valid locations a player can perform an action in
 */
void BitBoard::setPlayerMoves(int moveType)
{
    Board loc = turn == 0 ? players[0].board : players[1].board;
    setValidMoves(moveType, loc);
}

/*
 * bfs
 * Perform bfs from a given location to find all connected pieces of ice
 */
void BitBoard::bfs(Board loc, Board &vis)
{
    queue<Board> q;
    q.push(loc);

    while (!q.empty())
    {
        loc = q.front();
        q.pop();

        vis |= loc;

        Board up = getUp(loc) & board;
        Board upRight = getUpRight(loc) & board;
        Board right = getRight(loc) & board;
        Board down = getDown(loc) & board;
        Board downLeft = getDownLeft(loc) & board;
        Board left = getLeft(loc) & board;

        if (up && !(up & vis))
        {
            q.push(up);
        }

        if (upRight && !(upRight & vis))
        {
            q.push(upRight);
        }

        if (right && !(right & vis))
        {
            q.push(right);
        }

        if (down && !(down & vis))
        {
            q.push(down);
        }

        if (downLeft && !(downLeft & vis))
        {
            q.push(downLeft);
        }

        if (left && !(left & vis))
        {
            q.push(left);
        }
    }
}

/*
 * findRemaining
 * Find any separations in the glacier after a take action has been executed
 * Handles game winning logic regarding separation of the glacier
 */
void BitBoard::findRemaining()
{
    Board rem = board;
    players[turn].glacier = { 0, 0 };
    players[turn ^ 1].glacier = { 0, 0 };

    bfs(players[turn].board, players[turn].glacier);
    if (players[turn].glacier & players[turn ^ 1].board)
    {
        players[turn ^ 1].glacier = players[turn].glacier;
        rem ^= players[0].glacier;
    }
    else
    {
        bfs(players[turn ^ 1].board, players[turn ^ 1].glacier);
        rem ^= (players[0].glacier ^ players[1].glacier);
    }

    // Collecting extra ice, no winner
    if (rem)
    {
        board ^= rem;
        while (rem)
        {
            if (rem.top)
            {
                int idx = bitToIdx(rem.top);
                players[turn].ice += (glacierTop[idx] - '0');
                glacierTop[idx] = '0';
                rem.top ^= (1ULL << idx);
            }

            if (rem.bot)
            {
                int idx = bitToIdx(rem.bot);
                players[turn].ice += (glacierBot[idx] - '0');
                glacierBot[idx] = '0';
                rem.bot ^= (1ULL << idx);
            }
        }
    }

    // Winning move
    if (!(players[turn ^ 1].glacier & players[turn].glacier))
    {
        int count1 = getBitCount(players[turn].glacier.top) + getBitCount(players[turn].glacier.bot);
        int count2 = getBitCount(players[turn ^ 1].glacier.top) + getBitCount(players[turn ^ 1].glacier.bot);

        if (count1 > count2)
        {
            winner = turn;
            statement = winner == 0 ? "White wins by [surface area]" : "Black wins by [surface area]";
        }

        else if (count2 > count1)
        {
            winner = turn ^ 1;
            statement = winner == 0 ? "White wins by [surface area]" : "Black wins by [surface area]";
        }

        // TODO: check volumes
        else
        {
            players[turn].volume = 0;
            Board b = players[turn].glacier;
            while (b)
            {
                int idx = b.top ? bitToIdx(b.top) : bitToIdx(b.bot);
                b.top ? players[turn].volume += glacierTop[idx] : players[turn].volume += glacierBot[idx];
                b.top ? (b.top &= ~(1ULL << idx)) : (b.bot &= ~(1ULL << idx));
            }

            players[turn ^ 1].volume = 37 - players[turn].volume - players[turn].ice - players[turn ^ 1].ice;

            if (players[turn].volume > players[turn ^ 1].volume)
            {
                winner = turn;
                statement = winner == 0 ? "White wins by [volume]" : "Black wins by [volume]";
            }
            else if (players[turn ^ 1].volume > players[turn].volume)
            {
                winner = turn ^ 1;
                statement = winner == 0 ? "White wins by [volume]" : "Black wins by [volume]";
            }
            else
            {
                winner = players[turn].ice > players[turn ^ 1].ice ? turn : turn ^ 1;
                statement = winner == 0 ? "White wins by [ice collection]" : "Black wins by [ice collection]";
            }
        }
    }
}

/*
 * take
 * Performs a take action for the current player in a given direction
 */
void BitBoard::take(int dir)
{
    Board curr = players[turn].board;
    setValidMoves(TAKE, curr);

    switch (dir)
    {
        case UP:
        {
            curr = getUp(curr) & validMoves;

            while (curr)
            {
                int idx = curr.top ? bitToIdx(curr.top) : bitToIdx(curr.bot);
                curr.top ? glacierTop[idx]-- : glacierBot[idx]--;
                players[turn].ice++;
                (curr.top && glacierTop[idx] == '0' || curr.bot && glacierBot[idx] == '0') ? board ^= curr : board ^= 0;

                setValidMoves(TAKE, curr);
                curr = getUp(curr) & validMoves;
            }

            break;
        }

        case UP_RIGHT:
        {
            curr = getUpRight(curr) & validMoves;

            while (curr)
            {
                int idx = curr.top ? bitToIdx(curr.top) : bitToIdx(curr.bot);
                curr.top ? glacierTop[idx]-- : glacierBot[idx]--;
                players[turn].ice++;
                (curr.top && glacierTop[idx] == '0' || curr.bot && glacierBot[idx] == '0') ? board ^= curr : board ^= 0;

                setValidMoves(TAKE, curr);
                curr = getUpRight(curr) & validMoves;
            }

            break;
        }

        case RIGHT:
        {
            curr = getRight(curr) & validMoves;

            while (curr)
            {
                int idx = curr.top ? bitToIdx(curr.top) : bitToIdx(curr.bot);
                curr.top ? glacierTop[idx]-- : glacierBot[idx]--;
                players[turn].ice++;
                (curr.top && glacierTop[idx] == '0' || curr.bot && glacierBot[idx] == '0') ? board ^= curr : board ^= 0;

                setValidMoves(TAKE, curr);
                curr = getRight(curr) & validMoves;
            }

            break;
        }

        case DOWN:
        {
            curr = getDown(curr) & validMoves;

            while (curr)
            {
                int idx = curr.top ? bitToIdx(curr.top) : bitToIdx(curr.bot);
                curr.top ? glacierTop[idx]-- : glacierBot[idx]--;
                players[turn].ice++;
                (curr.top && glacierTop[idx] == '0' || curr.bot && glacierBot[idx] == '0') ? board ^= curr : board ^= 0;

                setValidMoves(TAKE, curr);
                curr = getDown(curr) & validMoves;
            }

            break;
        }

        case DOWN_LEFT:
        {
            curr = getDownLeft(curr) & validMoves;

            while (curr)
            {
                int idx = curr.top ? bitToIdx(curr.top) : bitToIdx(curr.bot);
                curr.top ? glacierTop[idx]-- : glacierBot[idx]--;
                players[turn].ice++;
                (curr.top && glacierTop[idx] == '0' || curr.bot && glacierBot[idx] == '0') ? board ^= curr : board ^= 0;

                setValidMoves(TAKE, curr);
                curr = getDownLeft(curr) & validMoves;
            }

            break;
        }

        case LEFT:
        {
            curr = getLeft(curr) & validMoves;

            while (curr)
            {
                int idx = curr.top ? bitToIdx(curr.top) : bitToIdx(curr.bot);
                curr.top ? glacierTop[idx]-- : glacierBot[idx]--;
                players[turn].ice++;
                (curr.top && glacierTop[idx] == '0' || curr.bot && glacierBot[idx] == '0') ? board ^= curr : board ^= 0;

                setValidMoves(TAKE, curr);
                curr = getLeft(curr) & validMoves;
            }

            break;
        }
    }
}

/*
 * place
 * Performs a place action for the current player in a given direction
 */
void BitBoard::place(int dir)
{
    Board curr = players[turn].board;
    setValidMoves(PLACE, curr);

    switch (dir)
    {
        case UP:
        {
            curr = getUp(curr) & validMoves;

            while (curr)
            {
                int idx = curr.top ? bitToIdx(curr.top) : bitToIdx(curr.bot);
                curr.top ? glacierTop[idx]++ : glacierBot[idx]++;
                players[turn].ice--;
                (curr.top && glacierTop[idx] == '1' || curr.bot && glacierBot[idx] == '1') ? board |= curr : board ^= 0;

                setValidMoves(PLACE, curr);
                curr = getUp(curr) & validMoves;
            }

            break;
        }

        case UP_RIGHT:
        {
            curr = getUpRight(curr) & validMoves;

            while (curr)
            {
                int idx = curr.top ? bitToIdx(curr.top) : bitToIdx(curr.bot);
                curr.top ? glacierTop[idx]++ : glacierBot[idx]++;
                players[turn].ice--;
                (curr.top && glacierTop[idx] == '1' || curr.bot && glacierBot[idx] == '1') ? board |= curr : board ^= 0;

                setValidMoves(PLACE, curr);
                curr = getUpRight(curr) & validMoves;
            }

            break;
        }

        case RIGHT:
        {
            curr = getRight(curr) & validMoves;

            while (curr)
            {
                int idx = curr.top ? bitToIdx(curr.top) : bitToIdx(curr.bot);
                curr.top ? glacierTop[idx]++ : glacierBot[idx]++;
                players[turn].ice--;
                (curr.top && glacierTop[idx] == '1' || curr.bot && glacierBot[idx] == '1') ? board |= curr : board ^= 0;

                setValidMoves(PLACE, curr);
                curr = getRight(curr) & validMoves;
            }

            break;
        }

        case DOWN:
        {
            curr = getDown(curr) & validMoves;

            while (curr)
            {
                int idx = curr.top ? bitToIdx(curr.top) : bitToIdx(curr.bot);
                curr.top ? glacierTop[idx]++ : glacierBot[idx]++;
                players[turn].ice--;
                (curr.top && glacierTop[idx] == '1' || curr.bot && glacierBot[idx] == '1') ? board |= curr : board ^= 0;

                setValidMoves(PLACE, curr);
                curr = getDown(curr) & validMoves;
            }

            break;
        }

        case DOWN_LEFT:
        {
            curr = getDownLeft(curr) & validMoves;

            while (curr)
            {
                int idx = curr.top ? bitToIdx(curr.top) : bitToIdx(curr.bot);
                curr.top ? glacierTop[idx]++ : glacierBot[idx]++;
                players[turn].ice--;
                (curr.top && glacierTop[idx] == '1' || curr.bot && glacierBot[idx] == '1') ? board |= curr : board ^= 0;

                setValidMoves(PLACE, curr);
                curr = getDownLeft(curr) & validMoves;
            }

            break;
        }

        case LEFT:
        {
            curr = getLeft(curr) & validMoves;

            while (curr)
            {
                int idx = curr.top ? bitToIdx(curr.top) : bitToIdx(curr.bot);
                curr.top ? glacierTop[idx]++ : glacierBot[idx]++;
                players[turn].ice--;
                (curr.top && glacierTop[idx] == '1' || curr.bot && glacierBot[idx] == '1') ? board |= curr : board ^= 0;

                setValidMoves(PLACE, curr);
                curr = getLeft(curr) & validMoves;
            }

            break;
        }
    }
}

/*
 * move
 * Performs a move action for the current player in a given direction
 */
void BitBoard::move(int dir)
{
    setValidMoves(MOVE, players[turn].board);

    switch (dir)
    {
        case UP:
        {
            players[turn].board = getUp(players[turn].board) & validMoves;
            break;
        }

        case UP_RIGHT:
        {
            players[turn].board = getUpRight(players[turn].board) & validMoves;
            break;
        }

        case RIGHT:
        {
            players[turn].board = getRight(players[turn].board) & validMoves;
            break;
        }

        case DOWN:
        {
            players[turn].board = getDown(players[turn].board) & validMoves;
            break;
        }

        case DOWN_LEFT:
        {
            players[turn].board = getDownLeft(players[turn].board) & validMoves;
            break;
        }

        case LEFT:
        {
            players[turn].board = getLeft(players[turn].board) & validMoves;
            break;
        }
    }

    if (winner == -1)
    {
        changeTurn();
    }
}

/*
 * play
 * Performs an action given the move type and direction
 */
void BitBoard::play(int moveType, int dir)
{
    if (dir == 6) dir = 0;

    if (winner == -1)
    {
        switch (moveType)
        {
            case TAKE:
            {
                statement = turn == 0 ? "White: Move" : "Black: Move";
                setPlayerMoves(TAKE);

                if (checkDirs()[dir])
                {
                    take(dir);
                    findRemaining();

                    if (winner != -1)
                    {
                        setPlayerMoves(MOVE);
                    }

                    lastMove = 0b10000000;
                    lastMove |= (dir << 3);
                }

                // printBoard();
                break;
            }

            case PLACE:
            {
                statement = turn == 0 ? "White: Move" : "Black: Move";
                setPlayerMoves(PLACE);

                if (checkDirs()[dir])
                {
                    place(dir);
                    setPlayerMoves(MOVE);
                    lastMove = 0b10000000;
                    lastMove |= 0b01000000;
                    lastMove |= (dir << 3);
                }

                // printBoard();
                break;
            }

            case MOVE:
            {
                statement = turn == 1 ? "White: Take/Place" : "Black: Take/Place";
                setPlayerMoves(MOVE);

                if (validMoves.top == 0 && validMoves.bot == 0)
                {
                    std::cout << "Game over!\n";
                    winner = turn ^ 1;
                }
                else
                {
                    if (checkDirs()[dir])
                    {
                        move(dir);
                        lastMove |= dir;
                    }
                }

                break;
            }
        }
    }
}

/*
 * playAction
 * Performs an action for the current player
 */
void BitBoard::playAction(Action move)
{
    switch (move & 0b01000000)
    {
        case 0b00000000:
            play(TAKE, ((move & 0b00111000) >> 3));
            break;

        case 0b01000000:
            play(PLACE, ((move & 0b00111000) >> 3));
            break;
    }

    play(MOVE, (move & 0b00000111));
}

/*
 * getGlacier
 * Return the glacier
 */
vector<char> BitBoard::getGlacier() const
{
    vector<char> v(NUM * NUM);
    memcpy(&v[0], &glacierBot[0], 60 * sizeof(char));
    memcpy(&v[60], &glacierTop[0], 61 * sizeof(char));

    return v;
}

/*
 * getWhitePlayer
 * Return the index of the white player
 */
int BitBoard::getWhitePlayer() const
{
    if (players[0].board.bot)
    {
        return bitToIdx(players[0].board.bot);
    }

    return 60 + bitToIdx(players[0].board.top);
}

/*
 * getBlackPlayer
 * Return the index of the black player
 */
int BitBoard::getBlackPlayer() const
{
    if (players[1].board.bot)
    {
        return bitToIdx(players[1].board.bot);
    }

    return 60 + bitToIdx(players[1].board.top);
}

/*
 * getWhiteIce
 * Return the amount of ice the white player has collected
 */
int BitBoard::getWhiteIce() const
{
    return players[0].ice;
}

/*
 * getBlackIce
 * Return the amount of ice the black player has collected
 */
int BitBoard::getBlackIce() const
{
    return players[1].ice;
}

/*
 * getWinner
 * Return the current winner (0 for white, 1 for black, -1 for no winner)
 */
int BitBoard::getWinner() const
{
    return winner;
}

/*
 * getStatement
 * Return the statement indicating the game state (current action to make)
 */
string BitBoard::getStatement() const
{
    return statement;
}

/*
 * getTurn
 * Return the current player's turn (0 for white, 1 for black, -1 for game over)
 */
int BitBoard::getTurn() const
{
    return turn;
}

/*
 * getTurns
 * Return the number of turns that have passed
 */
int BitBoard::getTurns() const
{
    return turns;
}

/*
 * getLastMove
 * Return the last action made
 */
Action BitBoard::getLastMove() const
{
    return lastMove;
}

/*
 * eval
 * Return a score indicating the current state/favor of the game
 */
int BitBoard::eval()
{
    int whiteGlacier = getBitCount(players[0].glacier.top) + getBitCount(players[0].glacier.bot);
    int blackGlacier = getBitCount(players[1].glacier.top) + getBitCount(players[1].glacier.bot);

    if (whiteGlacier != blackGlacier)
    {
        return (whiteGlacier > blackGlacier) ? (1000 - turns) : (-1000 + turns);
    }

    if (!(players[0].glacier & players[1].glacier))
    {
        return (players[0].volume > players[1].volume) ? (1000 - turns) : (players[0].volume < players[1].volume) ? (-1000 + turns) : (players[0].ice > players[1].ice) ? (1000 - turns) : (-1000 + turns);
    }

    Board whiteHalf = { 0, 0 };
    Board blackHalf = { 0, 0 };

    conBFS(players[0].board, whiteHalf, getNeighbors(players[1].board));
    conBFS(players[1].board, blackHalf, getNeighbors(players[0].board));

    int whiteSide = getBitCount(whiteHalf.top) + getBitCount(whiteHalf.bot);
    int blackSide = getBitCount(blackHalf.top) + getBitCount(blackHalf.bot);

    return 5 * (whiteSide - blackSide) + (players[0].ice - players[1].ice);
}


/*
 * conBFS
 * Perform a constrained BFS from a given location, terminating when a certain set of neighbors are touched
 */
void BitBoard::conBFS(Board loc, Board &vis, Board nbrs)
{
    queue<Board> q;
    q.push(loc);

    while (!q.empty())
    {
        loc = q.front();
        q.pop();

        vis |= loc;

        Board up = getUp(loc) & board;
        Board upRight = getUpRight(loc) & board;
        Board right = getRight(loc) & board;
        Board down = getDown(loc) & board;
        Board downLeft = getDownLeft(loc) & board;
        Board left = getLeft(loc) & board;

        if (up & nbrs)
        {
            return;
        }

        if (upRight & nbrs)
        {
            return;
        }

        if (right & nbrs)
        {
            return;
        }

        if (down & nbrs)
        {
            return;
        }

        if (downLeft & nbrs)
        {
            return;
        }

        if (left & nbrs)
        {
            return;
        }

        if (up && !(up & vis))
        {
            q.push(up);
        }

        if (upRight && !(upRight & vis))
        {
            q.push(upRight);
        }

        if (right && !(right & vis))
        {
            q.push(right);
        }

        if (down && !(down & vis))
        {
            q.push(down);
        }

        if (downLeft && !(downLeft & vis))
        {
            q.push(downLeft);
        }

        if (left && !(left & vis))
        {
            q.push(left);
        }
    }
}

/*
 * getValidActions
 * Find all valid actions for the current player and store them
 */
vector<Action> BitBoard::getValidActions()
{
    vector<Action> moves;
    setPlayerMoves(TAKE);
    vector<bool> t = checkDirs();

    for (int i = 0; i < 6; i++)
    {
        if (t[i])
        {
            BitBoard b(*this);
            b.play(TAKE, i);

            b.setPlayerMoves(MOVE);
            vector<bool> m = b.checkDirs();

            for (int j = 0; j < 6; j++)
            {
                if (m[j])
                {
                    Action move = 0b00000000;
                    move |= (i << 3);
                    move |= j;
                    moves.push_back(move);
                }
            }
        }
    }

    setPlayerMoves(PLACE);
    vector<bool> p = checkDirs();

    for (int i = 0; i < 6; i++)
    {
        if (p[i])
        {
            BitBoard b(*this);
            b.play(PLACE, i);

            b.setPlayerMoves(MOVE);
            vector<bool> m = b.checkDirs();

            for (int j = 0; j < 6; j++)
            {
                if (m[j])
                {
                    Action move = 0b01000000;
                    move |= (i << 3);
                    move |= j;
                    moves.push_back(move);
                }
            }
        }
    }

    return moves;
}

/*
 * reward
 * Return a score indicating the outcome of the game in the range [-1, 1]
 * For use with AlphaZero protocol which hasn't been implemented yet due to lack of knowledge
 *
 * float BitBoard::reward()
 * {
 *     int whiteGlacier = getBitCount(players[0].glacier.top) + getBitCount(players[0].glacier.bot);
 *     int blackGlacier = getBitCount(players[1].glacier.top) + getBitCount(players[1].glacier.bot);
 * 
 *     if (whiteGlacier != blackGlacier)
 *     {
 *         return (whiteGlacier > blackGlacier) ? 1.0 : -1.0;
 *     }
 * 
 *     if (!(players[0].glacier & players[1].glacier))
 *     {
 *         return (players[0].volume > players[1].volume) ? 1.0 : (players[0].volume < players[1].volume) ? -1.0 : (players[0].ice > players[1].ice) ? 1.0 : -1.0;
 *     }
 * 
 *     return 0.0;
 * }
 */

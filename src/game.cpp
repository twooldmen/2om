#include "include/game.h"
#include <fstream>
using std::ofstream;

bool botsPlay = false;

/*
 * getHexFromIdx
 * Return the hexagon coordinates converted from a given index on the BitBoard
 */
static vector<int> getHexFromIdx(int idx)
{
    int diff = idx - CNTR;

    int sgn = NUM - diff <= NUM / 2 ? -1 : 1;
    int a = diff % NUM;

    if (abs(a) > NUM / 2)
    {
        a = sgn * (NUM - abs(a));
    }

    return { a, int(round(float(diff) / float(NUM))) };
}

/*
 * hexToCart
 * Return the Cartesian coordinates converted from given hexagon coordinates
 */
static vector<int> hexToCart(int q, int r)
{
    int x = int(1.5 * HEX_RADIUS * q) + WIDTH / 2;
    int y = int(sqrt(3) / 2 * HEX_RADIUS * (2 * r + q)) + HEIGHT / 2;

    return { x, y };
}

/*
 * Constructor
 */
Game::Game()
{
    init();
}

/*
 * Destructor
 */
Game::~Game()
{

    delete window;
}

/*
 * init
 * Initialize all member variables
 */
void Game::init()
{
    window = nullptr;
    moveType = -1;
    font.loadFromFile("CourierPrime-Bold.ttf");

    window = new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), "Two Old Men Fight On A Glacier", sf::Style::Titlebar | sf::Style::Close);
    window->setPosition({ 0, 0 });
    window->setFramerateLimit(30);
    showValid = false;
}

/*
 * setBoard
 * Set the displayed game board to the given board state
 */
void Game::setBoard(const BitBoard &board)
{
    this->board = board;
}

/*
 * getWindowIsOpen
 * Return true if the window is open, false otherwise
 */
const bool Game::getWindowIsOpen() const
{

    return window->isOpen();
}

/*
 * pollEvent
 * Continuously check the user input and perform actions based on the input
 */
void Game::pollEvent()
{
    while (window->pollEvent(e))
    {
        switch (e.type)
        {
            case sf::Event::Closed:
            {
                window->close();
                break;
            }

            case sf::Event::KeyPressed:
            {
                if (e.key.code == sf::Keyboard::S)
                {
                    ofstream out;
                    out.open("last_game.txt");
                    for (int i = 0; i < history.size(); i++)
                    {
                        out << moveToString(history[i]) << "\n";
                    }
                    out.close();
                }

                break;
            }

            case sf::Event::MouseButtonPressed:
            {
                if (board.getWinner() == -1)
                {
                    vector<int> loc = { sf::Mouse::getPosition(*(window)).x, sf::Mouse::getPosition(*(window)).y };

                    if (loc[0] >= WIDTH / 2 - 100 && loc[0] <= WIDTH / 2 + 100 && loc[1] >= (HEIGHT - 150))
                    {
                        engine(board);
                        history.push_back(board.getLastMove());
                    }

                    // TAKE or PLACE button pressed
                    else if (moveType != MOVE)
                    {
                        // check if TAKE was pressed
                        if (loc[0] >= 0 && loc[0] <= 250 && loc[1] >= (HEIGHT - 150) && loc[1] <= HEIGHT)
                        {
                            moveType = TAKE;
                            board.setPlayerMoves(TAKE);
                            showValid = true;
                        }
                        // check if PLACE was pressed
                        else if (loc[0] >= (WIDTH - 300) && loc[0] <= WIDTH && loc[1] >= (HEIGHT - 150) && loc[1] <= HEIGHT)
                        {
                            moveType = PLACE;
                            board.setPlayerMoves(PLACE);
                            showValid = true;
                        }

                        // execute action
                        else
                        {
                            vector<bool> v = board.checkDirs();
                            int playerIdx = board.getTurn() == 0 ? board.getWhitePlayer() : board.getBlackPlayer();
                            vector<int> hexCoords = getHexFromIdx(120 - playerIdx);
                            vector<int> cartCoords = hexToCart(hexCoords[0], hexCoords[1]);

                            int xDiff = loc[0] - cartCoords[0], yDiff = loc[1] - cartCoords[1];

                            // up and down
                            if (abs(xDiff) <= HEX_RADIUS / 2)
                            {
                                // up
                                if (yDiff <= (-sqrt(3) * HEX_RADIUS + HEX_RADIUS / 2) && yDiff >= (-sqrt(3) * HEX_RADIUS - HEX_RADIUS / 2))
                                {
                                    if (v[0])
                                    {
                                        board.play(moveType, 6);
                                        moveType = MOVE;
                                    }
                                }

                                // down
                                else if (yDiff <= (sqrt(3) * HEX_RADIUS + HEX_RADIUS / 2) && yDiff >= (sqrt(3) * HEX_RADIUS - HEX_RADIUS / 2))
                                {
                                    if (v[3])
                                    {
                                        board.play(moveType, 3);
                                        moveType = MOVE;
                                    }
                                }
                            }

                            // up right and down right
                            if (xDiff >= HEX_RADIUS && xDiff <= 2 * HEX_RADIUS)
                            {
                                // up right
                                if (yDiff <= (-sqrt(3) / 2.0 * HEX_RADIUS + HEX_RADIUS / 2) && yDiff >= (-sqrt(3) / 2.0 * HEX_RADIUS - HEX_RADIUS / 2))
                                {
                                    if (v[1])
                                    {
                                        board.play(moveType, 1);
                                        moveType = MOVE;
                                    }
                                }

                                // down right
                                else if (yDiff <= (sqrt(3) / 2.0 * HEX_RADIUS + HEX_RADIUS / 2) && yDiff >= (sqrt(3) / 2.0 * HEX_RADIUS - HEX_RADIUS / 2))
                                {
                                    if (v[2])
                                    {
                                        board.play(moveType, 2);
                                        moveType = MOVE;
                                    }
                                }
                            }

                            // up right and down right
                            if (xDiff <= -HEX_RADIUS && xDiff >= -2 * HEX_RADIUS)
                            {
                                // up right
                                if (yDiff <= (-sqrt(3) / 2.0 * HEX_RADIUS + HEX_RADIUS / 2) && yDiff >= (-sqrt(3) / 2.0 * HEX_RADIUS - HEX_RADIUS / 2))
                                {
                                    if (v[5])
                                    {
                                        board.play(moveType, 5);
                                        moveType = MOVE;
                                    }
                                }

                                // down right
                                else if (yDiff <= (sqrt(3) / 2.0 * HEX_RADIUS + HEX_RADIUS / 2) && yDiff >= (sqrt(3) / 2.0 * HEX_RADIUS - HEX_RADIUS / 2))
                                {
                                    if (v[4])
                                    {
                                        board.play(moveType, 4);
                                        moveType = MOVE;
                                    }
                                }
                            }

                            if (board.getWinner() != -1)
                            {
                                history.push_back(board.getLastMove());
                            }
                        }
                    }

                    else if (moveType == MOVE && showValid)
                    {
                        vector<bool> v = board.checkDirs();
                        int playerIdx = board.getTurn() == 0 ? board.getWhitePlayer() : board.getBlackPlayer();
                        vector<int> hexCoords = getHexFromIdx(120 - playerIdx);
                        vector<int> cartCoords = hexToCart(hexCoords[0], hexCoords[1]);

                        int xDiff = loc[0] - cartCoords[0], yDiff = loc[1] - cartCoords[1];

                        // up and down
                        if (abs(xDiff) <= HEX_RADIUS / 2)
                        {
                            // up
                            if (yDiff <= (-sqrt(3) * HEX_RADIUS + HEX_RADIUS / 2) && yDiff >= (-sqrt(3) * HEX_RADIUS - HEX_RADIUS / 2))
                            {
                                if (v[0])
                                {
                                    board.play(moveType, 6);
                                    showValid = false;
                                    moveType = -1;
                                }
                            }

                            // down
                            else if (yDiff <= (sqrt(3) * HEX_RADIUS + HEX_RADIUS / 2) && yDiff >= (sqrt(3) * HEX_RADIUS - HEX_RADIUS / 2))
                            {
                                if (v[3])
                                {
                                    board.play(moveType, 3);
                                    showValid = false;
                                    moveType = -1;
                                }
                            }
                        }

                        // up right and down right
                        if (xDiff >= HEX_RADIUS && xDiff <= 2 * HEX_RADIUS)
                        {
                            // up right
                            if (yDiff <= (-sqrt(3) / 2.0 * HEX_RADIUS + HEX_RADIUS / 2) && yDiff >= (-sqrt(3) / 2.0 * HEX_RADIUS - HEX_RADIUS / 2))
                            {
                                if (v[1])
                                {
                                    board.play(moveType, 1);
                                    showValid = false;
                                    moveType = -1;
                                }
                            }

                            // down right
                            else if (yDiff <= (sqrt(3) / 2.0 * HEX_RADIUS + HEX_RADIUS / 2) && yDiff >= (sqrt(3) / 2.0 * HEX_RADIUS - HEX_RADIUS / 2))
                            {
                                if (v[2])
                                {
                                    board.play(moveType, 2);
                                    showValid = false;
                                    moveType = -1;
                                }
                            }
                        }

                        // up right and down right
                        if (xDiff <= -HEX_RADIUS && xDiff >= -2 * HEX_RADIUS)
                        {
                            // up right
                            if (yDiff <= (-sqrt(3) / 2.0 * HEX_RADIUS + HEX_RADIUS / 2) && yDiff >= (-sqrt(3) / 2.0 * HEX_RADIUS - HEX_RADIUS / 2))
                            {
                                if (v[5])
                                {
                                    board.play(moveType, 5);
                                    showValid = false;
                                    moveType = -1;
                                }
                            }

                            // down right
                            else if (yDiff <= (sqrt(3) / 2.0 * HEX_RADIUS + HEX_RADIUS / 2) && yDiff >= (sqrt(3) / 2.0 * HEX_RADIUS - HEX_RADIUS / 2))
                            {
                                if (v[4])
                                {
                                    board.play(moveType, 4);
                                    showValid = false;
                                    moveType = -1;
                                }
                            }
                        }

                        history.push_back(board.getLastMove());
                    }
                }

                break;
            }

            default:
            {
                // moveType = -1;
                break;
            }
        }
    }
}

/*
 * update
 * Update the game state
 */
void Game::update()
{
    pollEvent();
}

/*
 * render
 * Draw all objects on the window
 */
void Game::render()
{
    window->clear(WHITE);
    drawHexagons();
    drawGlacier();
    drawPlayers();
    drawIceCount();
    drawStatement();
    drawButtons();

    if (showValid && board.getWinner() == -1)
    {
        drawValidMoves();
    }

    window->display();

    // if (board.getTurn() == 0 && board.getWinner() == -1)
    // {
    //     engine(board);
    // }
}

/*
 * drawHexagons
 * Draw the hexagon board on the window
 */
void Game::drawHexagons()
{
    vector<int> hexCoords(2);
    vector<int> cartCoords(2);

    // Draw new frame
    for (int i = 0; i < NUM * NUM; i++)
    {
        hexCoords = getHexFromIdx(120 - i);
        cartCoords = hexToCart(hexCoords[0], hexCoords[1]);

        sf::CircleShape hexagon(HEX_RADIUS, 6);
        hexagon.setOutlineColor(GRAY);
        hexagon.setOutlineThickness(2);
        hexagon.setPosition(sf::Vector2f(cartCoords[0] - (1.0 / 3.0 * HEX_RADIUS), cartCoords[1] - (4.0 / 3.0 * HEX_RADIUS)));
        hexagon.rotate(30);

        window->draw(hexagon);
    }
}

/*
 * drawGlacier
 * Draw the glacier on the window
 */
void Game::drawGlacier()
{
    vector<int> hexCoords(2);
    vector<int> cartCoords(2);

    int whiteIdx = board.getWhitePlayer();
    int blackIdx = board.getBlackPlayer();

    vector<char> v = board.getGlacier();
    // Set size and color for all objects on the game board
    for (int i = 0; i < NUM * NUM; i++)
    {
        if (v[i] > '0')
        {
            hexCoords = getHexFromIdx(120 - i);
            cartCoords = hexToCart(hexCoords[0], hexCoords[1]);
            sf::CircleShape ice;
            ice.setRadius(ICE_RADIUS);
            ice.setFillColor(GLACIER);

            ice.setPosition(cartCoords[0] - ICE_RADIUS, cartCoords[1] - ICE_RADIUS);

            sf::Text iceCount;
            iceCount.setFont(font);
            iceCount.setFillColor(WHITE);
            iceCount.setOutlineColor(BLACK);
            iceCount.setCharacterSize(FONT_SIZE);
            iceCount.setOutlineThickness(2);

            iceCount.setString(v[i]);
            iceCount.setPosition(cartCoords[0] - iceCount.getGlobalBounds().width / 2.0, cartCoords[1] - iceCount.getGlobalBounds().height);

            window->draw(ice);
            window->draw(iceCount);
        }
    }
}

/*
 * drawPlayers
 * Draw the players on the window
 */
void Game::drawPlayers()
{
    vector<char> v = board.getGlacier();
    vector<int> hexCoords(2);
    vector<int> cartCoords(2);

    int whiteIdx = board.getWhitePlayer();
    int blackIdx = board.getBlackPlayer();

    sf::CircleShape whitePawn;
    sf::CircleShape blackPawn;
    sf::Text iceCount;

    iceCount.setFont(font);
    iceCount.setFillColor(WHITE);
    iceCount.setOutlineColor(BLACK);
    iceCount.setCharacterSize(FONT_SIZE);
    iceCount.setOutlineThickness(2);

    hexCoords = getHexFromIdx(120 - whiteIdx);
    cartCoords = hexToCart(hexCoords[0], hexCoords[1]);

    iceCount.setString(v[whiteIdx]);
    iceCount.setPosition(cartCoords[0] - iceCount.getGlobalBounds().width / 2.0, cartCoords[1] - iceCount.getGlobalBounds().height);

    whitePawn.setPosition(cartCoords[0] - PAWN_RADIUS, cartCoords[1] - PAWN_RADIUS);
    whitePawn.setRadius(PAWN_RADIUS);
    whitePawn.setFillColor(CREAM);

    window->draw(whitePawn);
    window->draw(iceCount);

    hexCoords = getHexFromIdx(120 - blackIdx);
    cartCoords = hexToCart(hexCoords[0], hexCoords[1]);

    blackPawn.setPosition(cartCoords[0] - PAWN_RADIUS, cartCoords[1] - PAWN_RADIUS);
    blackPawn.setRadius(PAWN_RADIUS);
    blackPawn.setFillColor(BLACK);

    iceCount.setString(v[blackIdx]);
    iceCount.setPosition(cartCoords[0] - iceCount.getGlobalBounds().width / 2.0, cartCoords[1] - iceCount.getGlobalBounds().height);

    window->draw(blackPawn);
    window->draw(iceCount);
}

/*
 * drawIceCount
 * Draw the player's ice collected amounts on the window
 */
void Game::drawIceCount()
{
    sf::Text whiteIce;
    sf::Text blackIce;

    whiteIce.setFont(font);
    whiteIce.setFillColor(BLACK);
    whiteIce.setCharacterSize((FONT_SIZE * 3) / 2);
    whiteIce.setString("White's Ice: " + to_string(board.getWhiteIce()));

    whiteIce.setPosition(10, 0);

    sf::FloatRect whiteBack = whiteIce.getGlobalBounds();
    sf::RectangleShape whiteRect(sf::Vector2f(whiteBack.width + 20, 2 * whiteBack.height));
    whiteRect.setPosition(sf::Vector2f(0, 0));
    whiteRect.setFillColor(WHITE);

    if (board.getTurn() == 0 && board.getWinner() == -1)
    {
        whiteRect.setFillColor(RED);
    }

    window->draw(whiteRect);
    window->draw(whiteIce);

    blackIce.setFont(font);
    blackIce.setFillColor(BLACK);
    blackIce.setCharacterSize((FONT_SIZE * 3) / 2);
    blackIce.setString("Black's Ice: " + to_string(board.getBlackIce()));

    blackIce.setPosition(WIDTH - blackIce.getGlobalBounds().width - 10, 0);

    sf::FloatRect blackBack = blackIce.getGlobalBounds();
    sf::RectangleShape blackRect(sf::Vector2f(blackBack.width + 20, 2 * blackBack.height));
    blackRect.setPosition(sf::Vector2f(WIDTH - blackBack.width - 20, 0));
    blackRect.setFillColor(WHITE);

    if (board.getTurn() == 1 && board.getWinner() == -1)
    {
        blackRect.setFillColor(RED);
    }

    window->draw(blackRect);
    window->draw(blackIce);
}

/*
 * drawStatement
 * Draw the current statement of the game state on the window
 */
void Game::drawStatement()
{
    sf::Text statement;
    statement.setFont(font);
    statement.setFillColor(BLACK);
    statement.setCharacterSize((FONT_SIZE * 3) / 2);
    statement.setString(board.getStatement());

    statement.setPosition(WIDTH / 2 - statement.getGlobalBounds().width / 2.0, HEX_RADIUS);

    sf::FloatRect winningBack = statement.getGlobalBounds();
    sf::RectangleShape winningRect(sf::Vector2f(winningBack.width + 20, 2 * winningBack.height));
    winningRect.setPosition(sf::Vector2f(WIDTH / 2 - winningBack.width / 2 - 10, HEX_RADIUS));
    winningRect.setFillColor(WHITE);

    window->draw(winningRect);
    window->draw(statement);
}

/*
 * drawButtons
 * Draw the take and place buttons on the window
 */
void Game::drawButtons()
{
    sf::Text takeButton;
    sf::Text placeButton;
    sf::Text botButton;

    takeButton.setFont(font);
    takeButton.setFillColor(BLACK);
    takeButton.setCharacterSize(FONT_SIZE * 2);
    takeButton.setString("TAKE");

    takeButton.setPosition(0, HEIGHT - (FONT_SIZE * 2 + 12));

    sf::FloatRect takeBack = takeButton.getGlobalBounds();
    sf::RectangleShape takeRect(sf::Vector2f(takeBack.width + 20, 2 * takeBack.height));
    takeRect.setPosition(sf::Vector2f(0, HEIGHT - (FONT_SIZE * 2 + 6)));
    takeRect.setFillColor(CREAM);

    placeButton.setFont(font);
    placeButton.setFillColor(BLACK);
    placeButton.setCharacterSize(FONT_SIZE * 2);
    placeButton.setString("PLACE");

    placeButton.setPosition(WIDTH - placeButton.getGlobalBounds().width - 16, HEIGHT - (FONT_SIZE * 2 + 12));

    sf::FloatRect placeBack = placeButton.getGlobalBounds();
    sf::RectangleShape placeRect(sf::Vector2f(placeBack.width + 20, 2 * placeBack.height));
    placeRect.setPosition(sf::Vector2f(WIDTH - placeBack.width - 20, HEIGHT - (FONT_SIZE * 2 + 6)));
    placeRect.setFillColor(CREAM);

    botButton.setFont(font);
    botButton.setFillColor(BLACK);
    botButton.setCharacterSize(FONT_SIZE * 2);
    botButton.setString("BOT");

    botButton.setPosition(WIDTH / 2 - botButton.getGlobalBounds().width / 2, HEIGHT - (FONT_SIZE * 2 + 12));

    sf::FloatRect botBack = botButton.getGlobalBounds();
    sf::RectangleShape botRect(sf::Vector2f(botBack.width + 20, 2 * botBack.height));
    botRect.setPosition(sf::Vector2f(WIDTH / 2 - botBack.width / 2 - 5, HEIGHT - (FONT_SIZE * 2 + 6)));
    botRect.setFillColor(CREAM);

    window->draw(takeRect);
    window->draw(takeButton);

    window->draw(placeRect);
    window->draw(placeButton);

    window->draw(botRect);
    window->draw(botButton);
}

/*
 * drawValidMoves
 * Draw the valid moves on the window
 */
void Game::drawValidMoves()
{
    board.setPlayerMoves(moveType);
    vector<bool> v = board.checkDirs();
    int playerIdx = board.getTurn() == 0 ? board.getWhitePlayer() : board.getBlackPlayer();

    vector<int> hexCoords = getHexFromIdx(120 - playerIdx);
    vector<int> cartCoords = hexToCart(hexCoords[0], hexCoords[1]);

    for (int i = 0; i < 6; i++)
    {
        if (v[i])
        {
            sf::CircleShape validMove;
            validMove.setPosition(cartCoords[0] - MOVE_RADIUS, cartCoords[1] - MOVE_RADIUS);
            validMove.setRadius(MOVE_RADIUS);
            moveType == MOVE ? validMove.setFillColor(BLUE) : moveType == PLACE ? validMove.setFillColor(RED) : validMove.setFillColor(GREEN);
            switch (i)
            {
                case 0:
                    validMove.move(0, -sqrt(3) * HEX_RADIUS);
                    window->draw(validMove);
                    break;
                case 1:
                    validMove.move(1.5 * HEX_RADIUS, -sqrt(3) / 2.0 * HEX_RADIUS);
                    window->draw(validMove);
                    break;
                case 2:
                    validMove.move(1.5 * HEX_RADIUS, sqrt(3) / 2.0 * HEX_RADIUS);
                    window->draw(validMove);
                    break;
                case 3:
                    validMove.move(0, sqrt(3) * HEX_RADIUS);
                    window->draw(validMove);
                    break;
                case 4:
                    validMove.move(-1.5 * HEX_RADIUS, sqrt(3) / 2.0 * HEX_RADIUS);
                    window->draw(validMove);
                    break;
                case 5:
                    validMove.move(-1.5 * HEX_RADIUS, -sqrt(3) / 2.0 * HEX_RADIUS);
                    window->draw(validMove);
                    break;
            }
        }
    }
}

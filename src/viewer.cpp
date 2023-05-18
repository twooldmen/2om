#include "include/viewer.h"

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
Viewer::Viewer(const string &fileName)
{
    init();
    ifstream in;
    in.open(fileName);

    while (!in.eof())
    {
        string s;
        in >> s;

        if (s[0] == 'T' || s[0] == 'P')
        {
            // convert s to Action
            Action a = 0b10000000;

            s[0] == 'T' ? a |= 0b00000000 : a |= 0b01000000;
            a |= ((s[1] - '0') << 3);
            a |= (s[3] - '0');

            history.push_back(a);
        }
    }

    std::cout << history.size() << "\n";
}

/*
 * Destructor
 */
Viewer::~Viewer()
{

    delete window;
}

/*
 * init
 * Initialize all member variables
 */
void Viewer::init()
{
    window = nullptr;
    font.loadFromFile("CourierPrime-Bold.ttf");

    window = new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), "Two Old Men Fight On A Glacier", sf::Style::Titlebar | sf::Style::Close);
    window->setPosition({ 0, 0 });
    window->setFramerateLimit(30);

    currTurn = 0;
}

/*
 * setBoard
 * Set the displayed game board to the given board state
 */
void Viewer::setBoard(const BitBoard &board)
{
    this->board = board;
}

/*
 * getWindowIsOpen
 * Return true if the window is open, false otherwise
 */
const bool Viewer::getWindowIsOpen() const
{

    return window->isOpen();
}

/*
 * pollEvent
 * Continuously check the user input and perform actions based on the input
 */
void Viewer::pollEvent()
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
                switch (e.key.code)
                {
                    case sf::Keyboard::Right:
                    {
                        if (currTurn < history.size())
                        {
                            board.playAction(history[currTurn]);
                            currTurn++;
                        }

                        break;
                    }

                    case sf::Keyboard::Left:
                    {
                        if (currTurn > 0)
                        {
                            currTurn--;
                            BitBoard tmp;

                            for (int i = 0; i < currTurn; i++)
                            {
                                tmp.playAction(history[i]);
                            }

                            board = tmp;
                        }

                        break;
                    }

                    default: break;
                }

                default: break;
            }
        }
    }
}

/*
 * update
 * Update the game state
 */
void Viewer::update()
{
    pollEvent();
}

/*
 * render
 * Draw all objects on the window
 */
void Viewer::render()
{
    window->clear(WHITE);
    drawHexagons();
    drawGlacier();
    drawPlayers();
    drawIceCount();
    drawStatement();

    window->display();
}

/*
 * drawHexagons
 * Draw the hexagon board on the window
 */
void Viewer::drawHexagons()
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
void Viewer::drawGlacier()
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
void Viewer::drawPlayers()
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
void Viewer::drawIceCount()
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
void Viewer::drawStatement()
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

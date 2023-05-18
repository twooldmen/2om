#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "constants.h"
#include "board.h"
#include "bot.h"

class Game
{
public:
    Game();
    ~Game();

    const bool getWindowIsOpen() const;
    void pollEvent();
    void update();
    void render();

    void setBoard(const BitBoard &board);

private:
    BitBoard board;

    sf::RenderWindow *window;
    sf::Event e;
    sf::Font font;
    bool showValid;

    int moveType;

    int currTurn;
    vector<Action> history;

    void init();

    void drawHexagons();
    void drawGlacier();
    void drawPlayers();
    void drawIceCount();
    void drawStatement();
    void drawButtons();
    void drawValidMoves();
};

#endif

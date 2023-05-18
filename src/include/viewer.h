#ifndef VIEWER_H
#define VIEWER_H

#include <fstream>
using std::ifstream;

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "constants.h"
#include "board.h"

class Viewer
{
public:
    Viewer(const string &fileName);
    ~Viewer();

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

    int currTurn;
    vector<Action> history;

    void init();

    void drawHexagons();
    void drawGlacier();
    void drawPlayers();
    void drawIceCount();
    void drawStatement();
};

#endif
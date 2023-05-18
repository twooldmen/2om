#ifndef CONSTANTS_H
#define CONSTANTS_H

const int NUM = 11;
const int CNTR = NUM / 2 * NUM + NUM / 2;

const int MAX_DEPTH = 64;

const int WIDTH = 1880;
const int HEIGHT = 1200;

const int HEX_RADIUS = HEIGHT / 24;
const int ICE_RADIUS = (27 * HEX_RADIUS) / 40;
const int PAWN_RADIUS = (21 * HEX_RADIUS) / 40;
const int MOVE_RADIUS = (11 * HEX_RADIUS) / 40;
const int FONT_SIZE = 24;

#define GLACIER sf::Color(169, 218, 231)
#define WHITE sf::Color(255, 255, 255)
#define BLACK sf::Color(0, 0, 0)
#define GRAY sf::Color(100, 100, 100)
#define CREAM sf::Color(252, 230, 184)
#define BLUE sf::Color(50, 50, 205)
#define RED sf::Color(205, 50, 50)
#define GREEN sf::Color(50, 205, 50)

#endif

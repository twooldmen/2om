#include "include/board.h"
#include "include/game.h"
#include "include/viewer.h"
#include "include/openings.h"
#include <fstream>
using std::ofstream;

#include <chrono>
#include <thread>

#include <tuple>
using std::tuple, std::make_tuple, std::get;

vector<tuple<int, string, string, int>> testOpenings(const map<Action, vector<Action>> &openings)
{
    int i = 1;
    vector<tuple<int, string, string, int>> results;

    for (auto keyVal : openings)
    {
        for (auto val : keyVal.second)
        {
            int turns = 1;
            BitBoard game;
            // Game display;

            // out << "Opening: " << moveToString(keyVal.first) << " " << moveToString(val) << "\n";
            // cout << "Opening: " << moveToString(keyVal.first) << " " << moveToString(val) << "\n";

            // out << "\t" << moveToString(keyVal.first) << "\t" << moveToString(val) << "\n";
            // cout << moveToString(keyVal.first) << "\t" << moveToString(val) << "\n";

            game.playAction(keyVal.first);
            game.playAction(val);

            // display.setBoard(game);
            // display.update();
            // display.render();

            while (game.getWinner() == -1)
            {
                if (game.getTurn() == 0)
                {
                    turns++;
                }

                engine(game);

                // out << "\t" << moveToString(game.getLastMove());

                // if (game.getTurn() == 1 && game.getWinner() == -1)
                // {
                //     out << "\t";
                // }
                // else
                // {
                //     out << "\n";
                // }

                // display.setBoard(game);
                // display.update();
                // display.render();
            }

            // out << game.getStatement() << " after " << turns << " turns\n\n";
            // cout << game.getStatement() << " after " << turns << " turns\n\n";

            results.push_back(std::make_tuple(turns, moveToString(keyVal.first), moveToString(val), game.getTurn()));
        }
    }

    // out.close();

    return results;
}

int main()
{

    // Main game loop
    Game game;

    while (game.getWindowIsOpen())
    {
        game.update();
        game.render();
    }

    // std::ofstream out;
    // out.open("openings.txt");

    // map<Action, vector<Action>> openings;
    // populateOpenings(openings);

    // vector<tuple<int, string, string, int>> results = testOpenings(openings);
    // sort(results.begin(), results.end(), std::greater<tuple<int, string, string, int>>());

    // out << "OPENINGS\n\n";
    // for (auto res: results)
    // {
    //     out << get<1>(res) << "\t" << get<2>(res) << "\t" << get<0>(res) << "\t";
    //     out << (get<3>(res) == 0 ? "White" : "Black") << "\n";
    // }

    // Viewer viewer1("game1.txt");
    // while (viewer1.getWindowIsOpen())
    // {
    //     viewer1.update();
    //     viewer1.render();
    // }

    // Viewer viewer2 = Viewer("game2.txt");
    // while (viewer2.getWindowIsOpen())
    // {
    //     viewer2.update();
    //     viewer2.render();
    // }

    return 0;
}

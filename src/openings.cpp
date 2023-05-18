#include "include/openings.h"

void populateOpenings(map<Action, vector<Action>> &openings)
{
    for (int i = 1; i <= 6; i++)
    {
        for (int j = 1; j <= 6; j++)
        {
            if (j != i)
            {
                Action move = 0b10000000;
                move |= (i << 3);
                move |= j;

                openings[move] = {};
            }
        }
    }

    for (auto keyVal : openings)
    {
        BitBoard b;
        b.playAction(keyVal.first);

        openings[keyVal.first] = b.getValidActions();
    }
}
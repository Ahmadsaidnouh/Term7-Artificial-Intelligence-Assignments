#include "A_star.h"
#include <iostream>

int main()
{
    string initialState = "835416270";
    string goalState    = "123804765";

    AStarSearch(initialState, goalState);

    return 0;
}

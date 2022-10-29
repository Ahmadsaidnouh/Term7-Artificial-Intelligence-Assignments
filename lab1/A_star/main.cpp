#include "A_star.h"
#include <string>
#include <iostream>

typedef tuple<vector<int>, long long, long long> rTuple;

int main()
{
    string initState = "125348670";
    string goalState = "012345678";
    int type = 0;

    rTuple results = AStarSearch(initState, goalState, type);

    vector<int> path = get<0>(results);
    for (int i = 0; i < path.size(); i++)
    {
        string s = "";
        if (to_string(path[i]).size() == 8)
        {
            s = "0" + to_string(path[i]);
            print2dVector(toVector2D(s));
            cout << endl;
        }
        else
        {
            print2dVector(toVector2D(to_string(path[i])));
            cout << endl;
        }
    }

    cout << get<1>(results) << endl;

    return 0;
}

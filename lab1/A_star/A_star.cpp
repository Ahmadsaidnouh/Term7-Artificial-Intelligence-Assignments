#include "utilities.h"
#include <cstring>
#include <cfloat>
#include <queue>
#include <set>
#include <map>
#include <iostream>
#include <unordered_map>

#define N 3 // 3x3 grid

typedef pair<int, string> frontierPair;     // (f, state)
typedef set<frontierPair> pQueue;           // frontier
typedef unordered_map<string, string> uMap; // (state, parent)

void AStarSearch(string initialState, string goalState)
{
    unordered_map<int, pair<int, int>> goalGrid;
    setGrid(goalGrid, goalState);

    pQueue frontier;
    int g = 0;
    int h = calculateManhattanHeuristic(initialState, goalGrid);
    int f = g + h;
    frontier.insert(make_pair(f, initialState));

    set<string> searchFrontier;
    searchFrontier.insert(initialState);

    uMap parentMap;
    parentMap[initialState] = initialState;

    set<string> explored;

    vector<frontierPair> neighbors; // (h, state)

    bool goalFound = false;

    while (!frontier.empty())
    {
        frontierPair p = *frontier.begin(); // pair of (f, state)
        frontier.erase(frontier.begin());
        string s = p.second;
        f = p.first;
        h = calculateManhattanHeuristic(s, goalGrid);
        g = f - h;

        explored.insert(s);

        if (isGoal(s, goalState))
        {
            cout << "goal state is found" << endl;
            goalFound = true;
            tracePath(parentMap, goalState);
            break;
        }

        findNeighbors(s, goalState, neighbors, goalGrid);
        int fNew = 0;
        for (int i = 0; i < neighbors.size(); i++)
        {
            fNew = g + neighbors[i].first + 1; // f = g + h
            if (!setSearch(explored, neighbors[i].second) && !setSearch(searchFrontier, neighbors[i].second))
            {

                frontier.insert(make_pair(fNew, neighbors[i].second)); // (f, state)
                searchFrontier.insert(neighbors[i].second);
                parentMap[neighbors[i].second] = p.second;
            }
            else if (!setSearch(explored, neighbors[i].second))
            {
                if (fNew < neighbors[i].first)
                {
                    frontier.insert(make_pair(fNew, neighbors[i].second));
                    parentMap[neighbors[i].second] = p.second;
                }
            }
        }
    }

    if (!goalFound)
    {
        cout << "failed to find the goal state" << endl;
    }

    return;
}

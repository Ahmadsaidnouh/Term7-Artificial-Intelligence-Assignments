#include "utilities.h"
#include <cstring>
#include <cfloat>
#include <queue>
#include <set>
#include <map>
#include <iostream>
#include <unordered_map>

#define N 3 // 3x3 grid

typedef pair<int, string> frontierPair;                                                   // (f, state)
typedef priority_queue<frontierPair, vector<frontierPair>, greater<frontierPair>> pQueue; // frontier
typedef unordered_map<string, string> uMap;                                               // (state, parent)
typedef tuple<vector<int>, long long, long long> rTuple;                                  // (path, explored, depth)

rTuple AStarSearch(string initialState, string goalState, int type)
{
    rTuple results;

    pQueue frontier;
    int g = 0;
    int h = calculateHeuristic(initialState, type);
    int f = g + h;
    frontier.push(make_pair(f, initialState));

    set<string> searchFrontier;
    searchFrontier.insert(initialState);

    uMap parentMap;
    parentMap[initialState] = initialState;

    unordered_map<string, double> costMap; // f
    costMap[initialState] = f;

    set<string> explored;

    vector<string> neighbors; // (h, state)

    while (!frontier.empty())
    {
        frontierPair p = frontier.top(); // pair of (f, state)
        frontier.pop();

        searchFrontier.erase(p.second);

        string s = p.second;
        f = p.first;
        h = calculateHeuristic(s, type);
        g = f - h;

        explored.insert(s);

        if (isGoal(s, goalState))
        {
            vector<int> path;
            cout << "goal state is found" << endl;
            path = tracePath(parentMap, goalState);
            get<0>(results) = path;
            get<1>(results) = explored.size();
            get<2>(results) = path.size() - 1;
            break;
        }

        neighbors = findNeighbors(s);
        int fNew = 0;
        int hNew = 0;
        for (int i = 0; i < neighbors.size(); i++)
        {
            hNew = calculateHeuristic(neighbors[i], type);
            fNew = g + hNew + 1; // f = g + h
            if (!setSearch(explored, neighbors[i]) && !setSearch(searchFrontier, neighbors[i]))
            {
                frontier.push(make_pair(fNew, neighbors[i])); // (f, state)
                searchFrontier.insert(neighbors[i]);
                parentMap[neighbors[i]] = p.second;
                costMap[neighbors[i]] = fNew;
            }
            else if (!setSearch(explored, neighbors[i]))
            {
                if (fNew < costMap[neighbors[i]])
                {
                    frontier.push(make_pair(fNew, neighbors[i]));
                    parentMap[neighbors[i]] = p.second;
                    costMap[neighbors[i]] = fNew;
                }
            }
        }
    }
    return results;
}

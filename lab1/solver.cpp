#include <iostream>
#include <algorithm>
#include <string.h>
#include <string>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>
#include <tuple>
#include <queue>
#include <stack>
#include <utility>
#include <emscripten.h>

#define N 3

using namespace std;

typedef pair<float, int> pi;
typedef vector<vector<int>> vector2d;
typedef unordered_map<string, string> uMap;
typedef pair<int, string> frontierPair;                                                   // (f, state)
typedef priority_queue<frontierPair, vector<frontierPair>, greater<frontierPair>> pQueue; // frontier
typedef unordered_map<string, string> uMap;                                               // (state, parent)
typedef tuple<vector<int>, long long, long long> rTuple;                                  // (path, explored, depth)

// start A* utility functions.
pair<int, int> stringToVectorPositions(int pos)
{
    unordered_map<int, pair<int, int>> positions;

    // take position of an element in state string
    // then return its position in a 2d vector
    positions[0] = {0, 0};
    positions[1] = {0, 1};
    positions[2] = {0, 2};
    positions[3] = {1, 0};
    positions[4] = {1, 1};
    positions[5] = {1, 2};
    positions[6] = {2, 0};
    positions[7] = {2, 1};
    positions[8] = {2, 2};

    return positions[pos];
}

pair<int, int> findZero(string initialState)
{
    int found = initialState.find('0');
    return stringToVectorPositions(found);
}

vector2d toVector2D(string state)
{
    static int index = 0;
    vector2d vState;
    for (int i = 0; i < N; i++)
    {
        vector<int> vRow;
        for (int j = 0; j < N; j++)
        {
            vRow.push_back(int(state[index]) - 48);
            index++;
        }
        vState.push_back(vRow);
    }
    index = 0;
    return vState;
}

int calculateHeuristic(string state, int type)
{
    unordered_map<int, pair<int, int>> goalPositions;
    goalPositions[0] = {0, 0};
    goalPositions[1] = {0, 1};
    goalPositions[2] = {0, 2};
    goalPositions[3] = {1, 0};
    goalPositions[4] = {1, 1};
    goalPositions[5] = {1, 2};
    goalPositions[6] = {2, 0};
    goalPositions[7] = {2, 1};
    goalPositions[8] = {2, 2};

    vector2d vState = toVector2D(state);

    int h = 0;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (vState[i][j] != 0)
            {
                pair<int, int> pos = goalPositions[vState[i][j]];
                if (type == 0)
                {
                    h += abs(i - pos.first) + abs(j - pos.second);
                }
                else
                {
                    h += sqrt((abs(i - pos.first) * abs(i - pos.first)) + (abs(j - pos.second) * abs(j - pos.second)));
                }
            }
        }
    }
    return h;
}

pair<string, pair<int, int>> checkDirections(string state)
{
    /*
                 (up)
                  |
   (left)----EmptySquare----(right)
                  |
                (down)
    */

    // returns a string of boolean [up,down,right,left] if string is 1101 so it can move in all directions except right direction

    string directionsFlag = "";

    vector2d v = toVector2D(state);

    pair<int, int> pos = findZero(state);
    int row = pos.first;
    int column = pos.second;

    bool flags[4];
    flags[0] = row != 0;    // up
    flags[1] = row != 2;    // down
    flags[2] = column != 2; // right
    flags[3] = column != 0; // left

    for (int i = 0; i < 4; i++)
    {
        if (flags[i])
        {
            directionsFlag += "1";
        }
        else
        {
            directionsFlag += "0";
        }
    }

    return make_pair(directionsFlag, make_pair(row, column));
}

string toString(vector2d state)
{
    string s = "";
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            s += to_string(state[i][j]);
        }
    }
    return s;
}

vector<string> findNeighbors(string state)
{
    vector<string> neighbors;

    pair<string, pair<int, int>> p;
    p = checkDirections(state);

    string possibleDirections = p.first;

    int zeroRow = p.second.first;
    int zeroColumn = p.second.second;

    vector2d vs = toVector2D(state);

    vector<pair<int, int>> directions;
    directions.push_back(make_pair(zeroRow - 1, zeroColumn)); // up
    directions.push_back(make_pair(zeroRow + 1, zeroColumn)); // down
    directions.push_back(make_pair(zeroRow, zeroColumn + 1)); // right
    directions.push_back(make_pair(zeroRow, zeroColumn - 1)); // left

    for (int i = 0; i < possibleDirections.size(); i++)
    {
        if (possibleDirections[i] == '1')
        {
            string str = "";
            str = state;
            vector2d vn = toVector2D(str);
            vn[zeroRow][zeroColumn] = vs[directions[i].first][directions[i].second];
            vn[directions[i].first][directions[i].second] = 0;
            str = toString(vn);
            neighbors.push_back(str);
        }
    }
    return neighbors;
}

bool setSearch(set<string> states, string state)
{
    auto pos = states.find(state);

    if (pos != states.end())
        return true;
    else
        return false;
}

vector<int> tracePath(uMap parentMap)
{
    vector<int> path;
    string goalState = "012345678";

    while (parentMap[goalState] != goalState)
    {
        path.push_back(stoi(goalState));
        goalState = parentMap[goalState];
    }

    path.push_back(stoi(goalState));

    return path;
}
// end A* utility functions.
// 
// 
// 
// start states utiility functions
string stringifyPath(rTuple result)
{
    // don't touch this code please ;_;

    vector<int> path = get<0>(result);
    long long expandedCount = get<1>(result);
    long long maxDepth = get<2>(result);

    int indexZero, oldR, oldC, newR, newC;
    string newZero, direction, currentZero, stringPath = "";
    for (unsigned long long i = path.size() - 1; i >= 1; --i)
    {
        if ((path[i] / 100000000) == 0)
        {
            indexZero = 0;
            oldR = 0;
            oldC = 0;
        }
        else
        {
            indexZero = to_string(path[i]).find('0');
            oldR = indexZero / 3;
            oldC = indexZero % 3;
        }
        currentZero = to_string(indexZero);

        if ((path[i - 1] / 100000000) == 0)
        {
            indexZero = 0;
            newR = 0;
            newC = 0;
        }
        else
        {
            indexZero = to_string(path[i - 1]).find('0');
            newR = indexZero / 3;
            newC = indexZero % 3;
        }
        newZero = to_string(indexZero);

        if (oldR > newR)
            direction = "down";
        else if (oldR < newR)
            direction = "up";
        else if (oldC > newC)
            direction = "right";
        else if (oldC < newC)
            direction = "left";

        stringPath += newZero + "-" + direction + "-" + currentZero + ",";
    }

    string response = stringPath + ";" + to_string(expandedCount) + ";" + to_string(maxDepth);

    return response;
}

// A utility function to count inversions in given array 'arr[]'
int getInvCount(int state)
{
    // don't touch this code please ;_;
    string strState = to_string(state);
    if ((state / 100000000) == 0)
        strState.insert(0, "0");

    int inv_count = 0;
    for (int i = 0; i < 9 - 1; i++)
        for (int j = i + 1; j < 9; j++)
            if ((strState[j] - 48) && (strState[i] - 48) && ((strState[i] - 48) > (strState[j] - 48)))
                inv_count++;

    return inv_count;
}

// This function returns true if given 8 puzzle is solvable.
bool isSolvable(int state)
{
    // don't touch this code please ;_;
    // Count inversions in given 8 puzzle
    int invCount = getInvCount(state);

    // return true if inversion count is even.
    return (invCount % 2 == 0);
}

vector<int> getNeighbors(int state)
{
    // don't touch this code please ;_;
    vector<int> neighbors;
    string strState = to_string(state);

    if ((state / 100000000) == 0)
        strState.insert(0, "0");

    int indexZero = strState.find('0');
    int rowZero = indexZero / 3;
    int columnZero = indexZero % 3;

    int rowTmp, colTmp, newIndexZero;

    string sTemp = "";
    rowTmp = rowZero - 1;
    colTmp = columnZero;
    if ((rowTmp <= 2) && (rowTmp >= 0) && (colTmp <= 2) && (colTmp >= 0))
    {
        sTemp = strState;
        newIndexZero = rowTmp * 3 + colTmp;
        sTemp[indexZero] = sTemp[newIndexZero];
        sTemp[newIndexZero] = '0';
        neighbors.push_back(stoi(sTemp));
    }

    sTemp = "";
    rowTmp = rowZero + 1;
    colTmp = columnZero;
    if ((rowTmp <= 2) && (rowTmp >= 0) && (colTmp <= 2) && (colTmp >= 0))
    {
        sTemp = strState;
        newIndexZero = rowTmp * 3 + colTmp;
        sTemp[indexZero] = sTemp[newIndexZero];
        sTemp[newIndexZero] = '0';
        neighbors.push_back(stoi(sTemp));
    }

    sTemp = "";
    rowTmp = rowZero;
    colTmp = columnZero - 1;
    if ((rowTmp <= 2) && (rowTmp >= 0) && (colTmp <= 2) && (colTmp >= 0))
    {
        sTemp = strState;
        newIndexZero = rowTmp * 3 + colTmp;
        sTemp[indexZero] = sTemp[newIndexZero];
        sTemp[newIndexZero] = '0';
        neighbors.push_back(stoi(sTemp));
    }

    sTemp = "";
    rowTmp = rowZero;
    colTmp = columnZero + 1;
    if ((rowTmp <= 2) && (rowTmp >= 0) && (colTmp <= 2) && (colTmp >= 0))
    {
        sTemp = strState;
        newIndexZero = rowTmp * 3 + colTmp;
        sTemp[indexZero] = sTemp[newIndexZero];
        sTemp[newIndexZero] = '0';
        neighbors.push_back(stoi(sTemp));
    }

    return neighbors;
}
// end states utiility functions
// 
// 
// start states solving functions
rTuple solveBFS(int initState)
{
string initialState = to_string(initState);
    if (initialState.size() == 8)
    {
        initialState = "0" + initialState;
    }

    rTuple results;   
    queue<string> frontier;
    frontier.push(initialState);
    set<string> searchFrontier;
    searchFrontier.insert(initialState);
    uMap parentMap;
    parentMap[initialState] = initialState;

    set<string> explored;
    vector<string> neighbors;

    while (!frontier.empty())
    {
        string p = frontier.front();
        frontier.pop();

        searchFrontier.erase(p);

        string s = p;

        explored.insert(s);

        if (s == "012345678")
        {
            vector<int> path;
            path = tracePath(parentMap);
            results = make_tuple(path, explored.size(), path.size() - 1);
            break;
        }

        neighbors = findNeighbors(s);
        for (int i = 0; i < neighbors.size(); i++)
        {
            if (!setSearch(explored, neighbors[i]) && !setSearch(searchFrontier, neighbors[i]))
            {
                frontier.push(neighbors[i]);
                searchFrontier.insert(neighbors[i]);
                parentMap[neighbors[i]] = p;
            }
        }
    }
    return results;
}

rTuple solveDFS(int initState)
{
    string initialState = to_string(initState);
    if (initialState.size() == 8)
    {
        initialState = "0" + initialState;
    }

    rTuple results;   
    stack<string> frontier;
    frontier.push(initialState);
    set<string> searchFrontier;
    searchFrontier.insert(initialState);
    uMap parentMap;
    parentMap[initialState] = initialState;

    set<string> explored;
    vector<string> neighbors;

    while (!frontier.empty())
    {
        string p = frontier.top();
        frontier.pop();

        searchFrontier.erase(p);

        string s = p;

        explored.insert(s);

        if (s == "012345678")
        {
            vector<int> path;
            path = tracePath(parentMap);
            results = make_tuple(path, explored.size(), path.size() - 1);
            break;
        }

        neighbors = findNeighbors(s);
        for (int i = 0; i < neighbors.size(); i++)
        {
            if (!setSearch(explored, neighbors[i]) && !setSearch(searchFrontier, neighbors[i]))
            {
                frontier.push(neighbors[i]);
                searchFrontier.insert(neighbors[i]);
                parentMap[neighbors[i]] = p;
            }
        }
    }
    return results;
}

rTuple solveAStar(int initState, int type)
{
    string initialState = to_string(initState);
    if (initialState.size() == 8)
    {
        initialState = "0" + initialState;
    }

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

        if (s == "012345678")
        {
            vector<int> path;
            // cout << "goal state is found" << endl;
            path = tracePath(parentMap);
            // get<0>(results) = path;
            // get<1>(results) = explored.size();
            // get<2>(results) = path.size() - 1;
            results = make_tuple(path, explored.size(), path.size() - 1);
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
// end states solving functions
// 
// 
// start main driver code
extern "C"
{
    EMSCRIPTEN_KEEPALIVE
    void solve(int initialState, int algorithmType)
    {
        // don't touch this code please ;_;
        string result = "";
        rTuple path;
        bool isInitialStateSolvable = isSolvable(initialState);
        
        if (isInitialStateSolvable)
        {
            switch (algorithmType)
            {
            case 1:
                path = solveBFS(initialState);
                break;
            case 2:
                path = solveDFS(initialState);
                break;
            case 3:
                // path = AStarSearch(tmp, 0);
                path = solveAStar(initialState, 0);
                break;
            case 4:
                // path = AStarSearch(tmp, 1);
                path = solveAStar(initialState, 1);
                break;
            default:
                // path = solveAStar(initialState, 1);
                path = solveAStar(initialState, 1);
                break;
            }

            result = stringifyPath(path);
        }
        else
        {
            result = "unsolvable";
        }
        string script = "SolutionPanel('" + result + "')";
        const char *Schar = script.c_str();
        emscripten_run_script(Schar);
    }
}
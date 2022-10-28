#include "utilities.h"
#include <stack>
#include <iostream>
#include <unordered_map>

#define N 3

typedef pair<int, string> frontierPair;

void setGrid(unordered_map<int, pair<int, int>> &positions, string goalState)
{
    static int k = 0;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            positions[int(goalState[k]) - 48] = {i, j};
            k++;
        }
    }
}

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

bool isGoal(string state, string goalState)
{
    if (state == goalState)
        return true;
    else
        return false;
}

void print2dVector(vector2d vState)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cout << vState[i][j];
            cout << " ";
        }
        cout << endl;
    }
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

int calculateManhattanHeuristic(string state, unordered_map<int, pair<int, int>> goalGrid)
{
    vector2d vState = toVector2D(state);

    int h = 0;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (vState[i][j] != 0)
            {
                pair<int, int> pos = goalGrid[vState[i][j]];
                h += abs(i - pos.first) + abs(j - pos.second);
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

void findNeighbors(string state, string goalState, vector<frontierPair> &neighbors, unordered_map<int, pair<int, int>> goalGrid)
{
    neighbors.clear();

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
            int h = calculateManhattanHeuristic(str, goalGrid);
            neighbors.push_back(make_pair(h, str));
        }
    }
}

bool setSearch(set<string> states, string state)
{
    auto pos = states.find(state);

    if (pos != states.end())
        return true;
    else
        return false;
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

void tracePath(uMap parentMap, string goalState)
{
    stack<string> Path;

    while (parentMap[goalState] != goalState)
    {
        Path.push(goalState);
        goalState = parentMap[goalState];
    }

    Path.push(goalState);
    while (!Path.empty())
    {
        string p = Path.top();
        Path.pop();
        cout << endl;
        cout << p << endl;
        print2dVector(toVector2D(p));
    }

    return;
}
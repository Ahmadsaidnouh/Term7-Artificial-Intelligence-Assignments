#include "utilities.h"
#include <stack>
#include <iostream>

#define N 3

typedef pair<int, string> frontierPair;

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

int calculateManhattanHeuristic(string state, string goalState)
{
    vector2d vState = toVector2D(state);
    vector2d vGoalState = toVector2D(goalState);

    int h = 0;
    for (int state_i = 0; state_i < N; state_i++)
    {
        for (int state_j = 0; state_j < N; state_j++)
        {
            for (int goal_i = 0; goal_i < N; goal_i++)
            {
                for (int goal_j = 0; goal_j < N; goal_j++)
                {
                    if (vState[state_i][state_j] == vGoalState[goal_i][goal_j] && vState[state_i][state_j] != 0)
                    {
                        h += abs(state_i - goal_i) + abs(state_j - goal_j);
                    }
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

    int row = 0, column = 0;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (v[i][j] == 0)
            {
                if (i != 0)
                {
                    directionsFlag += "1";
                }
                else
                {
                    directionsFlag += "0";
                }

                if (i != 2)
                {
                    directionsFlag += "1";
                }
                else
                {
                    directionsFlag += "0";
                }

                if (j != 2)
                {
                    directionsFlag += "1";
                }
                else
                {
                    directionsFlag += "0";
                }

                if (j != 0)
                {
                    directionsFlag += "1";
                }
                else
                {
                    directionsFlag += "0";
                }
                row = i;
                column = j;
                break;
            }
        }
    }
    return make_pair(directionsFlag, make_pair(row, column));
}

void findNeighbors(string state, string goalState, vector<frontierPair> &neighbors)
{
    neighbors.clear();

    pair<string, pair<int, int>> p;
    p = checkDirections(state);

    string possibleDirections = p.first;

    int zeroRow = p.second.first;
    int zeroColumn = p.second.second;

    vector2d vs = toVector2D(state);

    if (possibleDirections[0] == '1') // up
    {
        string str = "";
        str = state;
        vector2d vn = toVector2D(str);
        vn[zeroRow][zeroColumn] = vs[zeroRow - 1][zeroColumn];
        vn[zeroRow - 1][zeroColumn] = 0;
        str = toString(vn);
        int h = calculateManhattanHeuristic(str, goalState);
        neighbors.push_back(make_pair(h, str));
    }

    if (possibleDirections[1] == '1') // down
    {
        string str = "";
        str = state;
        vector2d vn = toVector2D(str);
        vn[zeroRow][zeroColumn] = vs[zeroRow + 1][zeroColumn];
        vn[zeroRow + 1][zeroColumn] = 0;
        str = toString(vn);
        int h = calculateManhattanHeuristic(str, goalState);
        neighbors.push_back(make_pair(h, str));
    }

    if (possibleDirections[2] == '1') // right
    {
        string str = "";
        str = state;
        vector2d vn = toVector2D(str);
        vn[zeroRow][zeroColumn] = vs[zeroRow][zeroColumn + 1];
        vn[zeroRow][zeroColumn + 1] = 0;
        str = toString(vn);
        int h = calculateManhattanHeuristic(str, goalState);
        neighbors.push_back(make_pair(h, str));
    }

    if (possibleDirections[3] == '1') // left
    {
        string str = state;
        vector2d vn = toVector2D(str);
        vn[zeroRow][zeroColumn] = vs[zeroRow][zeroColumn - 1];
        vn[zeroRow][zeroColumn - 1] = 0;
        str = toString(vn);
        int h = calculateManhattanHeuristic(str, goalState);
        neighbors.push_back(make_pair(h, str));
    }
}

bool setSearch(set<string> states, string state)
{
    auto pos = states.find(state);

    // print element if it is present in set
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
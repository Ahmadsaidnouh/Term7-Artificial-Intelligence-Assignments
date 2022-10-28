#include <iostream>
#include <algorithm>
#include <string.h>
#include <string>
#include <cmath>
#include <vector>
#include <emscripten.h>

using namespace std;

string stringifyPath(vector<int> path)
{
    // don't touch this code please ;_;
    int indexZero, oldR, oldC, newR, newC;
    string newZero, direction, currentZero, stringPath = "";
    for (unsigned long long i = 0; i < path.size() - 1; i++)
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

        if ((path[i + 1] / 100000000) == 0)
        {
            indexZero = 0;
            newR = 0;
            newC = 0;
        }
        else
        {
            indexZero = to_string(path[i + 1]).find('0');
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
    return stringPath;
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

vector<int> findNeighbors(int state)
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

vector<int> solveBFS(int initialState)
{
    vector<int> finalPath;
    // code here

    return finalPath;
}

vector<int> solveDFS(int initialState)
{
    vector<int> finalPath;
    // code here

    return finalPath;
}

vector<int> solveAStar(int initialState, int type)
{
    vector<int> finalPath;
    // code here
    // type = 0 ==> manhattan
    // type = 1 ==> euclidean

    return finalPath;
}

extern "C"
{
    EMSCRIPTEN_KEEPALIVE
    void solve(int initialState, int algorithmType)
    {
        // don't touch this code please ;_;
        string result = "";
        vector<int> path;
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
                path = solveAStar(initialState, 0);
                break;
            case 4:
                path = solveAStar(initialState, 1);
                break;
            default:
                path = solveAStar(initialState, 1);
                break;
            }
            // The following three lines are for testing only.
            // Once you have a ready algorithm that return vector of string in path, delete them.
            // path.push_back(125340678);
            // path.push_back(120345678);
            // path.push_back(102345678);
            // path.push_back(12345678);
            path.push_back(125348670);
            path.push_back(125340678);
            path.push_back(120345678);
            path.push_back(102345678);
            path.push_back(12345678);

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
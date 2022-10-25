#include <iostream>
#include <algorithm>
#include <string.h>
#include <string>
#include <cmath>
#include <vector>
#include <emscripten.h>

using namespace std;

string stringifyPath(vector<string> path)
{
    int indexZero, oldR, oldC, newR, newC;
    string newZero, direction, currentZero, stringPath = "";
    for (unsigned long long i = 0; i < path.size() - 1; i++)
    {
        indexZero = path[i].find('0');
        oldR = indexZero / 3;
        oldC = indexZero % 3;
        currentZero = "slot" + to_string(indexZero);

        indexZero = path[i+1].find('0');
        newR = indexZero / 3;
        newC = indexZero % 3;
        newZero = "slot" + to_string(indexZero);
     
        if(oldR > newR) 
            direction = "down";
        else if(oldR < newR) 
            direction = "up";
        else if(oldC > newC) 
            direction = "right";
        else if(oldC < newC) 
            direction = "left";

        stringPath += newZero + "-" + direction + "-" + currentZero + ",";
    }
    // stringPath += ";";
    return stringPath;
}

string parseInitialState(int withoutZero, int indexOfZero)
{
    string res = to_string(withoutZero);
    return res.insert(indexOfZero, "0");
}

vector<string> calcRowColumnValues(string state)
{
    vector<string> res;
    return res;
}

// A utility function to count inversions in given array 'arr[]'
int getInvCount(string state)
{
    int inv_count = 0;
    for (int i = 0; i < 9 - 1; i++)
        for (int j = i + 1; j < 9; j++)
            if ((state[j] - 48) && (state[i] - 48) && ((state[i] - 48) > (state[j] - 48)))
                inv_count++;

    return inv_count;
}

// This function returns true if given 8 puzzle is solvable.
bool isSolvable(string state)
{
    // Count inversions in given 8 puzzle
    int invCount = getInvCount(state);

    // return true if inversion count is even.
    return (invCount % 2 == 0);
}

vector<string> findNeighbors(string state)
{
    vector<string> neighbors;

    int indexZero = state.find('0');
    int rowZero = indexZero / 3;
    int columnZero = indexZero % 3;

    int rowTmp, colTmp, newIndexZero;

    state.erase(indexZero);

    string sTemp = "";
    rowTmp = rowZero - 1;
    colTmp = columnZero;
    if ((rowTmp <= 2) && (rowTmp >= 0) && (colTmp <= 2) && (colTmp >= 0))
    {
        sTemp = state;
        newIndexZero = rowTmp * 3 + colTmp;
        sTemp.insert(newIndexZero, "0");
        neighbors.push_back(sTemp);
    }

    sTemp = "";
    rowTmp = rowZero + 1;
    colTmp = columnZero;
    if ((rowTmp <= 2) && (rowTmp >= 0) && (colTmp <= 2) && (colTmp >= 0))
    {
        sTemp = state;
        newIndexZero = rowTmp * 3 + colTmp;
        sTemp.insert(newIndexZero, "0");
    }

    sTemp = "";
    rowTmp = rowZero;
    colTmp = columnZero - 1;
    if ((rowTmp <= 2) && (rowTmp >= 0) && (colTmp <= 2) && (colTmp >= 0))
    {
        sTemp = state;
        newIndexZero = rowTmp * 3 + colTmp;
        sTemp.insert(newIndexZero, "0");
    }

    sTemp = "";
    rowTmp = rowZero;
    colTmp = columnZero + 1;
    if ((rowTmp <= 2) && (rowTmp >= 0) && (colTmp <= 2) && (colTmp >= 0))
    {
        sTemp = state;
        newIndexZero = rowTmp * 3 + colTmp;
        sTemp.insert(newIndexZero, "0");
    }

    return neighbors;
}

vector<string> solveBFS(string initialState)
{
    vector<string> finalPath;
    // code here

    return finalPath;
}

vector<string> solveDFS(string initialState)
{
    vector<string> finalPath;
    // code here

    return finalPath;
}

vector<string> solveAStar(string initialState, int type)
{
    vector<string> finalPath;
    // code here
    // type = 0 ==> manhattan
    // type = 1 ==> euclidean

    return finalPath;
}

extern "C"
{
    EMSCRIPTEN_KEEPALIVE
    void solve(int withoutZero, int indexOfZero, int algorithmType)
    {

        string initialState = parseInitialState(withoutZero, indexOfZero);
        string result = "";
        vector<string> path;
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
                path = solveBFS(initialState);
                break;
            }

            path.push_back("125340678");
            path.push_back("120345678");
            path.push_back("102345678");
            path.push_back("012345678");
            // path.push_back("012456783");
            // path.push_back("102456783");
            // path.push_back("152406783");
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
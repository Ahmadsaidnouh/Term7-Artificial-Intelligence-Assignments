#include <iostream>
#include <algorithm>
#include <string.h>
#include <string>
#include <cmath>
#include <vector>
#include <emscripten.h>

using namespace std;

string parseInitialState(int withoutZero, int indexOfZero)
{
    string res = to_string(withoutZero);
    return res.insert(indexOfZero, "0");
}

vector<string> calcRowColumnValues(string state) {
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



string solveBFS(string initialState)
{
    // code here
    return "BFS";
}

string solveDFS(string initialState)
{
    // code here
    return "DFS";
}

string solveAStar(string initialState, int type)
{
    // code here
    // type = 0 ==> manhattan
    // type = 1 ==> euclidean
    return type ? "Euclidean" : "Manhattan";
}

extern "C"
{
    EMSCRIPTEN_KEEPALIVE
    void solve(int withoutZero, int indexOfZero, int algorithmType)
    {

        string initialState = parseInitialState(withoutZero, indexOfZero);
        string result = "";
        bool isInitialStateSolvable = isSolvable(initialState);

        if (isInitialStateSolvable)
        {
            switch (algorithmType)
            {
            case 1:
                result = solveBFS(initialState);
                break;
            case 2:
                result = solveDFS(initialState);
                break;
            case 3:
                result = solveAStar(initialState, 0);
                break;
            case 4:
                result = solveAStar(initialState, 1);
                break;
            default:
                result = solveBFS(initialState);
                break;
            }
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
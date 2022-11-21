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


using namespace std;

unordered_map<long long, long long> policyMap;
unordered_map<long long, vector<pair<long long, long long>>> nodes;
unordered_map<long long, tuple<long long, long long, long long, long long, unordered_map<long long, vector<pair<long long, long long>>>>> tree; // alpha beta score action neighbors
long long root;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// start utility functions////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// returns all possible neighbors of a state
vector<long long> getNeighbors(long long state, bool player)
{
    // player = 0 ==> red
    // player = 1 ==> green
    vector<short int> c(7);
    vector<char> lastR(7);
    short int temp;
    for (int i = 0; i < 7; i++)
    {
        temp = (511 & (state >> i*9));
        c[i] = temp;
        lastR[i] = (448 & temp) >> 6;
    }

    vector<long long> neighbors;
    long long neighbor, shifter = 511;
    
    for (int i = 0; i < 7; i++)
    {
        neighbor = state;
        shifter = 511;
        if(lastR[i] < 6)
        {
            if(player)
            {
                int newCValue = 1 << lastR[i]; // to get 2^level
                long long newC = c[i] | newCValue; // sets the new place
                int newLast = lastR[i] + 1; // increment lastPlace
                newC = newC & (~ (7 << 6)); // reset the bits of the lastPlace
                newC = newC | (newLast << 6); // player the bits with the new values and now the state is ready
                neighbor = neighbor & (~ (shifter << i*9)); // reset the bits of the specified column
                neighbor = neighbor | (newC << i*9);
            }
            else
            {
                int newCValue = 1 << lastR[i]; // to get 2^level
                long long newC = c[i] & (~ newCValue); // resets the new place
                int newLast = lastR[i] + 1; // increment lastPlace
                newC = newC & (~ (7 << 6)); // reset the bits of the lastPlace
                newC = newC | (newLast << 6); // player the bits with the new values and now the state is ready
                neighbor = neighbor & (~ (shifter << i*9)); // reset the bits of the specified column
                neighbor = neighbor | (newC << i*9);
            }
            neighbors.push_back(neighbor);
        }
    }

    return neighbors;
}

// stringifies the result so that we can pass it to javascript
string stringify(long long state, int algorithmType)
{
    long long pastTemp = state;
    long long newTemp = policyMap[state];
    char c1, c2;
    short int row, column;
    for (int i = 0; i < 7; i++)
    {
        // to get the row and column of the new inserted block
        c1 = ((newTemp >> i*9) & 511) >> 6;
        c2 = ((pastTemp >> i*9) & 511) >> 6;
        if(c1 != c2)
        {
            row = c2;
            column = 6 - i;
            break;
        }
    }

    long long root = state;
    long long er = state;

    string res = "";
    res += (to_string(row) + "," + to_string(column) + "|");
    vector<long long> qu;
    res += (to_string(root) + ";");
    if(algorithmType != 1) // with alpha beta
    {
        res += (to_string(get<0>(tree[root])) + ";");
        res += (to_string(get<1>(tree[root])) + ";");
    }
    res += (to_string(get<2>(tree[root])) + ";");
    res += (to_string(get<3>(tree[root])) + ";");
    for (auto as : get<4>(tree[root])[root])
    {
        qu.push_back(as.first);
        res += (to_string(as.first) + ";");
    }
    res += "|";
    
    while(!qu.empty())
    {
        long long temp = qu[0];
        res += (to_string(temp) + ";");
        if(algorithmType != 1) // with alpha beta
        {
            res += (to_string(get<0>(tree[temp])) + ";");
            res += (to_string(get<1>(tree[temp])) + ";");
        }
        res += (to_string(get<2>(tree[temp])) + ";");
        res += (to_string(get<3>(tree[temp])) + ";");
        qu.erase(qu.begin());
        vector<pair<long long, long long>> ass=  get<4>(tree[temp])[temp];
        for (auto as : ass)
        {
            res += (to_string(as.first) + ";");
            qu.push_back(as.first);
        }
        res += "|";
    }
    return res;
}

// takes state as a string and returns a long long integer representing that string state. 
// This is much efficient
long long convertToBits(string state)
{
    short int c, b;
    bool first;
    char last;
    short int col;
    long long result = 0;

    int m = state.size() - 1;
    for (int i = 0; i < 7; i++)
    {
        first = true;
        last = 6;
        col = 0;
        for (int j = 0; j < 6; j++)
        {
            c = state[m--] - 48;
            if(c == 1)
                b = 1;
            else
                b = 0;
                
            col = (b << j) | col;
            if(first && c == 2)
            {
                first = false;
                last = j;
            }
        }
        col = (last << 6) | col;
        result = ((long long)col << i*9) | result;
    }
    
    return result;


}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// end utility functions//////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// start solving functions////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int heuristic(long long state, long long pastState, char player)
{
    // player = 1 ==> green
    // player = 2 ==> red
    char opponent = (player == 1) ? 2 : 1;

    vector<short int> c(7);
    vector<char> lastR(7);
    short int arr[6][7];
    short int temp;
    int r;
    for (int i = 0; i < 7; i++)
    {
        temp = (511 & (state >> i*9));
        c[i] = temp;
        lastR[i] = (448 & temp) >> 6;
    }
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            r = (c[i] >> j) & 1;
            
            if(j < lastR[i])
            {
                if(r == 0)
                    arr[j][6-i] = 2;
                else
                    arr[j][6-i] = 1;
            }
            else
                arr[j][6-i] = 0;

        }        
    }
    
    char c1, c2;
    short int row, column;
    for (int i = 0; i < 7; i++)
    {
        // to get the row and column of the new inserted block
        c1 = ((state >> i*9) & 511) >> 6;
        c2 = ((pastState >> i*9) & 511) >> 6;
        if(c1 != c2)
        {
            row = c2;
            column = 6 - i;
            break;
        }
    }

    // update row and column to match the garded matrix 
    row += 3;
    column += 3;
    
    short int guardedArr[12][13];
    for (int j = 0; j < 12; j++)
    {
        // build the guarded matrix
        for (int i = 0; i < 13; i++)
        {
            if((j >= 3 && j <= 8) && (i >= 3 && i <= 9))
                guardedArr[j][i] = arr[j-3][i-3];
            else
                guardedArr[j][i] = -1;
        }
    }
    int p1, p2, p3, p4, p5, p6, p7, p8, p9;
    pair<char, char> i1, i2, i3, i4, i5;
// ***************************************************************
// ********************** start Feature 1 ************************
// ***************************************************************
    for (int i = 0; i < 4; i++)
    {
        if(i == 0) 
        {
            i1.first = row;
            i1.second = column - 3;
            i2.first = row;
            i2.second = column - 2;
            i3.first = row;
            i3.second = column - 1;
            i4.first = row;
            i4.second = column;
        }
        else if(i == 1) 
        {
            i1.first = row - 3;
            i1.second = column;
            i2.first = row - 2;
            i2.second = column;
            i3.first = row - 1;
            i3.second = column;
            i4.first = row;
            i4.second = column;
        }
        else if(i == 2) 
        {
            i1.first = row - 3;
            i1.second = column - 3;
            i2.first = row - 2;
            i2.second = column - 2;
            i3.first = row - 1;
            i3.second = column - 1;
            i4.first = row;
            i4.second = column;
        }
        else if(i == 3) 
        {
            i1.first = row + 3;
            i1.second = column - 3;
            i2.first = row + 2;
            i2.second = column - 2;
            i3.first = row + 1;
            i3.second = column - 1;
            i4.first = row;
            i4.second = column;
        }
        for (int j = 0; j < 4; j++)
        {
            if(i == 0) 
            {
                p1 = guardedArr[i1.first][i1.second++];
                p2 = guardedArr[i2.first][i2.second++];
                p3 = guardedArr[i3.first][i3.second++];
                p4 = guardedArr[i4.first][i4.second++];
            }
            else if(i == 1) 
            {
                p1 = guardedArr[i1.first++][i1.second];
                p2 = guardedArr[i2.first++][i2.second];
                p3 = guardedArr[i3.first++][i3.second];
                p4 = guardedArr[i4.first++][i4.second];
            }
            else if(i == 2) 
            {
                p1 = guardedArr[i1.first++][i1.second++];
                p2 = guardedArr[i2.first++][i2.second++];
                p3 = guardedArr[i3.first++][i3.second++];
                p4 = guardedArr[i4.first++][i4.second++];
            }
            else if(i == 3) 
            {
                p1 = guardedArr[i1.first--][i1.second++];
                p2 = guardedArr[i2.first--][i2.second++];
                p3 = guardedArr[i3.first--][i3.second++];
                p4 = guardedArr[i4.first--][i4.second++];
            }
            if((player == p1)  && (p1 == p2) && (p2 == p3) && (p3 == p4))
                return (player == 1) ? INT_MAX : INT_MIN;
        }
    }
// ***************************************************************
// ********************** end Feature 1 **************************
// ***************************************************************
    
// ***************************************************************
// ********************** start Feature 2 win ********************
// ***************************************************************
    for (int i = 0; i < 3; i++)
    {
        if(i == 0) 
        {
            i1.first = row;
            i1.second = column - 3;
            i2.first = row;
            i2.second = column - 2;
            i3.first = row;
            i3.second = column - 1;
            i4.first = row;
            i4.second = column;
            i5.first = row;
            i5.second = column + 1;
        }
        else if(i == 1) 
        {
            i1.first = row - 3;
            i1.second = column - 3;
            i2.first = row - 2;
            i2.second = column - 2;
            i3.first = row - 1;
            i3.second = column - 1;
            i4.first = row;
            i4.second = column;
            i5.first = row + 1;
            i5.second = column + 1;
        }
        else if(i == 2) 
        {
            i1.first = row + 3;
            i1.second = column - 3;
            i2.first = row + 2;
            i2.second = column - 2;
            i3.first = row + 1;
            i3.second = column - 1;
            i4.first = row;
            i4.second = column;
            i5.first = row - 1;
            i5.second = column + 1;
        }

        for (int j = 0; j < 3; j++)
        {
            short int t1, t2;
            t1 = guardedArr[(i1.first - 1) < 0 ? 0 : (i1.first - 1)][i1.second];
            t2 = guardedArr[(i5.first - 1) < 0 ? 0 : (i5.first - 1)][i5.second];
            if(i == 0) 
            {
                p1 = guardedArr[i1.first][i1.second++];
                p2 = guardedArr[i2.first][i2.second++];
                p3 = guardedArr[i3.first][i3.second++];
                p4 = guardedArr[i4.first][i4.second++];
                p5 = guardedArr[i5.first][i5.second++];
            }
            else if(i == 1) 
            {
                p1 = guardedArr[i1.first++][i1.second++];
                p2 = guardedArr[i2.first++][i2.second++];
                p3 = guardedArr[i3.first++][i3.second++];
                p4 = guardedArr[i4.first++][i4.second++];
                p5 = guardedArr[i5.first++][i5.second++];
            }
            else if(i == 2) 
            {
                p1 = guardedArr[i1.first--][i1.second++];
                p2 = guardedArr[i2.first--][i2.second++];
                p3 = guardedArr[i3.first--][i3.second++];
                p4 = guardedArr[i4.first--][i4.second++];
                p5 = guardedArr[i5.first--][i5.second++];
            }

            if((player == p2) && (p2 == p3) && (p3 == p4))
                if((p1 != -1 && p5 != -1) && (p1 == 0 && p5 == 0))
                    if((t1 != 0) && (t2 != 0))
                        return (player == 1) ? INT_MAX : INT_MIN;
        }
    }
// ***************************************************************
// ********************** end Feature 2 win **********************
// ***************************************************************
    
// ***************************************************************
// ********************** start Feature 2 lose *******************
// ***************************************************************
    short int rowTemp = row, columnTemp = column;
    if(guardedArr[row+1][column-1] == opponent || guardedArr[row][column-1] == opponent)
    {
        for (int i = 0; i < 2; i++)
        {
            row = rowTemp;
            column = columnTemp;
            if(i == 0)
            {
                if(guardedArr[row+1][column-1] == opponent)
                    row++;
                else
                    continue;
            }
            else
            {
                if(guardedArr[row][column-1] != opponent)
                    continue;
            }
            column--;
            for (int i = 0; i < 3; i++)
            {
                if(i == 0) 
                {
                    i1.first = row;
                    i1.second = column - 3;
                    i2.first = row;
                    i2.second = column - 2;
                    i3.first = row;
                    i3.second = column - 1;
                    i4.first = row;
                    i4.second = column;
                    i5.first = row;
                    i5.second = column + 1;
                }
                else if(i == 1) 
                {
                    i1.first = row - 3;
                    i1.second = column - 3;
                    i2.first = row - 2;
                    i2.second = column - 2;
                    i3.first = row - 1;
                    i3.second = column - 1;
                    i4.first = row;
                    i4.second = column;
                    i5.first = row + 1;
                    i5.second = column + 1;
                }
                else if(i == 2) 
                {
                    i1.first = row + 3;
                    i1.second = column - 3;
                    i2.first = row + 2;
                    i2.second = column - 2;
                    i3.first = row + 1;
                    i3.second = column - 1;
                    i4.first = row;
                    i4.second = column;
                    i5.first = row - 1;
                    i5.second = column + 1;
                }
    
                for (int j = 0; j < 3; j++)
                {
                    short int t1, t2;
                    t1 = guardedArr[(i1.first - 1) < 0 ? 0 : (i1.first - 1)][i1.second];
                    t2 = guardedArr[(i5.first - 1) < 0 ? 0 : (i5.first - 1)][i5.second];
                    if(i == 0) 
                    {
                        p1 = guardedArr[i1.first][i1.second++];
                        p2 = guardedArr[i2.first][i2.second++];
                        p3 = guardedArr[i3.first][i3.second++];
                        p4 = guardedArr[i4.first][i4.second++];
                        p5 = guardedArr[i5.first][i5.second++];
                    }
                    else if(i == 1) 
                    {
                        p1 = guardedArr[i1.first++][i1.second++];
                        p2 = guardedArr[i2.first++][i2.second++];
                        p3 = guardedArr[i3.first++][i3.second++];
                        p4 = guardedArr[i4.first++][i4.second++];
                        p5 = guardedArr[i5.first++][i5.second++];
                    }
                    else if(i == 2) 
                    {
                        p1 = guardedArr[i1.first--][i1.second++];
                        p2 = guardedArr[i2.first--][i2.second++];
                        p3 = guardedArr[i3.first--][i3.second++];
                        p4 = guardedArr[i4.first--][i4.second++];
                        p5 = guardedArr[i5.first--][i5.second++];
                    }
                    if((opponent == p2) && (p2 == p3) && (p3 == p4))
                        if((p1 != -1 && p5 != -1) && (p1 == 0 && p5 == 0))
                            if((t1 != 0) && (t2 != 0))
                                return (opponent == 1) ? INT_MAX : INT_MIN;
                }
            } 
        } 
        
        
    }
    
    if(guardedArr[row+1][column+1] == opponent || guardedArr[row][column+1] == opponent)
    {
        for (int i = 0; i < 2; i++)
        {
            row = rowTemp;
            column = columnTemp;
            if(i == 0)
            {
                if(guardedArr[row+1][column+1] == opponent)
                    row++;
                else
                    continue;
            }
            else
            {
                if(guardedArr[row][column+1] != opponent)
                    continue;
            }
            column++;
            for (int i = 0; i < 3; i++)
            {
                if(i == 0) 
                {
                    i1.first = row;
                    i1.second = column - 3;
                    i2.first = row;
                    i2.second = column - 2;
                    i3.first = row;
                    i3.second = column - 1;
                    i4.first = row;
                    i4.second = column;
                    i5.first = row;
                    i5.second = column + 1;
                }
                else if(i == 1) 
                {
                    i1.first = row - 3;
                    i1.second = column - 3;
                    i2.first = row - 2;
                    i2.second = column - 2;
                    i3.first = row - 1;
                    i3.second = column - 1;
                    i4.first = row;
                    i4.second = column;
                    i5.first = row + 1;
                    i5.second = column + 1;
                }
                else if(i == 2) 
                {
                    i1.first = row + 3;
                    i1.second = column - 3;
                    i2.first = row + 2;
                    i2.second = column - 2;
                    i3.first = row + 1;
                    i3.second = column - 1;
                    i4.first = row;
                    i4.second = column;
                    i5.first = row - 1;
                    i5.second = column + 1;
                }

                for (int j = 0; j < 3; j++)
                {
                    short int t1, t2;
                    t1 = guardedArr[(i1.first - 1) < 0 ? 0 : (i1.first - 1)][i1.second];
                    t2 = guardedArr[(i5.first - 1) < 0 ? 0 : (i5.first - 1)][i5.second];
                    if(i == 0) 
                    {
                        p1 = guardedArr[i1.first][i1.second++];
                        p2 = guardedArr[i2.first][i2.second++];
                        p3 = guardedArr[i3.first][i3.second++];
                        p4 = guardedArr[i4.first][i4.second++];
                        p5 = guardedArr[i5.first][i5.second++];
                    }
                    else if(i == 1) 
                    {
                        p1 = guardedArr[i1.first++][i1.second++];
                        p2 = guardedArr[i2.first++][i2.second++];
                        p3 = guardedArr[i3.first++][i3.second++];
                        p4 = guardedArr[i4.first++][i4.second++];
                        p5 = guardedArr[i5.first++][i5.second++];
                    }
                    else if(i == 2) 
                    {
                        p1 = guardedArr[i1.first--][i1.second++];
                        p2 = guardedArr[i2.first--][i2.second++];
                        p3 = guardedArr[i3.first--][i3.second++];
                        p4 = guardedArr[i4.first--][i4.second++];
                        p5 = guardedArr[i5.first--][i5.second++];
                    }
                    if((opponent == p2) && (p2 == p3) && (p3 == p4))
                        if((p1 != -1 && p5 != -1) && (p1 == 0 && p5 == 0))
                            if((t1 != 0) && (t2 != 0))
                                return (opponent == 1) ? INT_MAX : INT_MIN;
                }
            }   
        }
    }
    row = rowTemp;
    column = columnTemp;
    if(lastR[6 - (column-3)] > 3)
    {
        p1 = guardedArr[row-1][column];
        p2 = guardedArr[row-2][column];
        p3 = guardedArr[row-3][column];
        if((opponent == p1) && (p1 == p2) && (p2 == p3))
                return (player == 1) ? INT_MAX : INT_MIN;
    }
    // *******************************
    p1 = guardedArr[row-1][column];
    p2 = guardedArr[row-2][column];
    p3 = guardedArr[row-3][column];
    if((opponent == p1) && (p1 == p2) && (p2 == p3))
            return (player == 1) ? INT_MAX : INT_MIN;
    p1 = guardedArr[row+1][column+1];
    p2 = guardedArr[row+2][column+2];
    p3 = guardedArr[row+3][column+3];
    if((opponent == p1) && (p1 == p2) && (p2 == p3))
            return (player == 1) ? INT_MAX : INT_MIN;
    p1 = guardedArr[row-1][column-1];
    p2 = guardedArr[row-2][column-2];
    p3 = guardedArr[row-3][column-3];
    if((opponent == p1) && (p1 == p2) && (p2 == p3))
            return (player == 1) ? INT_MAX : INT_MIN;
    p1 = guardedArr[row+1][column-1];
    p2 = guardedArr[row+2][column-2];
    p3 = guardedArr[row+3][column-3];
    if((opponent == p1) && (p1 == p2) && (p2 == p3))
            return (player == 1) ? INT_MAX : INT_MIN;
    p1 = guardedArr[row-1][column+1];
    p2 = guardedArr[row-2][column+2];
    p3 = guardedArr[row-3][column+3];
    if((opponent == p1) && (p1 == p2) && (p2 == p3))
            return (player == 1) ? INT_MAX : INT_MIN;
// ***************************************************************
// ********************** end Feature 2 lose *********************
// ***************************************************************
    long long score = 0;
// ***************************************************************
// ********************** start Feature 2 win 900000 *************
// ***************************************************************
    for (int i = 0; i < 3; i++)
    {
        if(i == 0) 
        {
            i1.first = row;
            i1.second = column - 3;
            i2.first = row;
            i2.second = column - 2;
            i3.first = row;
            i3.second = column - 1;
            i4.first = row;
            i4.second = column;
            i5.first = row;
            i5.second = column + 1;
        }
        else if(i == 1) 
        {
            i1.first = row - 3;
            i1.second = column - 3;
            i2.first = row - 2;
            i2.second = column - 2;
            i3.first = row - 1;
            i3.second = column - 1;
            i4.first = row;
            i4.second = column;
            i5.first = row + 1;
            i5.second = column + 1;
        }
        else if(i == 2) 
        {
            i1.first = row + 3;
            i1.second = column - 3;
            i2.first = row + 2;
            i2.second = column - 2;
            i3.first = row + 1;
            i3.second = column - 1;
            i4.first = row;
            i4.second = column;
            i5.first = row - 1;
            i5.second = column + 1;
        }

        for (int j = 0; j < 3; j++)
        {
            short int t1, t2;
            t1 = guardedArr[(i1.first - 1) < 0 ? 0 : (i1.first - 1)][i1.second];
            t2 = guardedArr[(i5.first - 1) < 0 ? 0 : (i5.first - 1)][i5.second];
            if(i == 0) 
            {
                p1 = guardedArr[i1.first][i1.second++];
                p2 = guardedArr[i2.first][i2.second++];
                p3 = guardedArr[i3.first][i3.second++];
                p4 = guardedArr[i4.first][i4.second++];
                p5 = guardedArr[i5.first][i5.second++];
            }
            else if(i == 1) 
            {
                p1 = guardedArr[i1.first++][i1.second++];
                p2 = guardedArr[i2.first++][i2.second++];
                p3 = guardedArr[i3.first++][i3.second++];
                p4 = guardedArr[i4.first++][i4.second++];
                p5 = guardedArr[i5.first++][i5.second++];
            }
            else if(i == 2) 
            {
                p1 = guardedArr[i1.first--][i1.second++];
                p2 = guardedArr[i2.first--][i2.second++];
                p3 = guardedArr[i3.first--][i3.second++];
                p4 = guardedArr[i4.first--][i4.second++];
                p5 = guardedArr[i5.first--][i5.second++];
            }
            if((player == p2) && (p2 == p3) && (p3 == p4))
                if((p1 == 0 && t1 != 0) || (p5 == 0 && t2 != 0))
                    (player == 1) ? (score += 900000) : (score -= 900000);
        }
    }
    if(lastR[6 - (column-3)] > 2)
    {
        p1 = guardedArr[row][column];
        p2 = guardedArr[row-1][column];
        p3 = guardedArr[row-2][column];
        if((player == p1) && (p1 == p2) && (p2 == p3))
                (player == 1) ? (score += 900000) : (score -= 900000);
    }
// ***************************************************************
// ********************** end Feature 2 win 900000 ***************
// ***************************************************************


// ***************************************************************
// ********************** start Feature 3 win ********************
// ***************************************************************
    i1.first = row;
    i1.second = column - 2;
    i2.first = row;
    i2.second = column - 1;
    i3.first = row;
    i3.second = column;
    i4.first = row;
    i4.second = column + 1;
    int maxMoves = 0, tempMoves;
    for (int i = 0; i < 2; i++)
    {
        pair<char, char> temp1 = i1;
        pair<char, char> temp2 = i4;
        short int t1, t2;
        tempMoves = 0;
        t1 = guardedArr[(i1.first - 1) < 0 ? 0 : (i1.first - 1)][i1.second];
        t2 = guardedArr[(i4.first - 1) < 0 ? 0 : (i4.first - 1)][i4.second];
        p1 = guardedArr[i1.first][i1.second++];
        p2 = guardedArr[i2.first][i2.second++];
        p3 = guardedArr[i3.first][i3.second++];
        p4 = guardedArr[i4.first][i4.second++];
        if(p2 != p3)
            continue;
        while((p1 == 0 && t1 != 0))
        {
            tempMoves++;
            temp1.second--;
            t1 = guardedArr[(temp1.first - 1) < 0 ? 0 : (temp1.first - 1)][temp1.second];
            p1 = guardedArr[temp1.first][temp1.second];
        }
        maxMoves = max(maxMoves, tempMoves);
        tempMoves = 0;
        while((p4 == 0 && t2 != 0))
        {
            tempMoves++;
            temp2.second++;
            t2 = guardedArr[(temp2.first - 1) < 0 ? 0 : (temp2.first - 1)][temp2.second];
            p4 = guardedArr[temp2.first][temp2.second];
        }
        maxMoves = max(maxMoves, tempMoves);
    }
    (player == 1) ? (score += (maxMoves*10000)) : (score -= (maxMoves*10000));
// ***************************************************************
// ************************ end Feature 3 win ********************
// ***************************************************************


// ***************************************************************
// ********************** start Feature 4 win ********************
// ***************************************************************
    column -= 3;
    short int scoreMat[6][7]= {
        {3, 4, 5, 7, 5, 4, 3},
        {4, 6, 8, 10, 8, 6, 4},
        {5, 8, 11, 13, 11,  8, 5 },
        {5, 8, 11, 13, 11,  8, 5 },
        {4, 6, 8 , 10, 8 , 6 ,4 },
        {3, 4, 5 , 7 , 5 , 4 ,3 }
    };

    if(column == 3)
        (player == 1) ? (score += 200) : (score -= 200);
    else if(column == 0 || column == 6)
        (player == 1) ? (score += 40) : (score -= 40);
    else if(column == 1 || column == 5)
        (player == 1) ? (score += 70) : (score -=70);
    else
        (player == 1) ? (score += 120) : (score -= 120);

    (player == 1) ? (score += scoreMat[row - 3][column]) : (score -= scoreMat[row - 3][column]);
    
    column += 3;
// ***************************************************************
// ************************ end Feature 4 win ********************
// ***************************************************************
    return score;  
}

pair<long long,long long> minimaxAB(long long state, long long pastState, int depth, long long alpha, long long beta, bool maximizingPlayer, int emptySlots)
{
    char player = maximizingPlayer ? 2 : 1;
    if (depth == 0 || emptySlots == 0) // or game is over in position
    {
        long long score = heuristic(state, pastState, player);
        get<0>(tree[state]) = alpha;
        get<1>(tree[state]) = beta;
        get<2>(tree[state]) = score;
        return {score, state};
    }

    if (maximizingPlayer)
    {
        pair<long long, long long> maxEval, eval; // pair carries score, state;
        maxEval.first = INT_MIN;
        vector<long long> neighbors = getNeighbors(state, player);
        long long chosenAction;
        for(auto neighbor : neighbors)
        {
            eval = minimaxAB(neighbor, state, depth - 1, alpha, beta, false, emptySlots - 1);
            if(eval.first >= maxEval.first)
                chosenAction = neighbor;
            maxEval = (maxEval.first > eval.first) ? maxEval : eval;

            alpha = max(alpha, eval.first);

            nodes[state].push_back({neighbor, eval.first});

            if(beta <= alpha)
                break;
        }
        get<0>(tree[state]) = alpha;
        get<1>(tree[state]) = beta;
        get<2>(tree[state]) = maxEval.first;
        get<3>(tree[state]) = chosenAction;
        get<4>(tree[state]) = nodes;

        policyMap[state] = chosenAction;
        return maxEval;
    }
    else
    {
        pair<long long, long long> minEval, eval; // score, state;
        minEval.first = INT_MAX;
        vector<long long> neighbors = getNeighbors(state, !player);
        long long chosenAction;
        for(auto neighbor : neighbors)
        {
            eval = minimaxAB(neighbor, state, depth - 1, alpha, beta, true, emptySlots - 1);
            if(eval.first <= minEval.first)
                chosenAction = neighbor;
            
            minEval = (minEval.first < eval.first) ? minEval : eval;
            beta = min(beta, eval.first);
            
            nodes[state].push_back({neighbor, eval.first});
            if(beta <= alpha)
                break;
        }

        get<0>(tree[state]) = alpha;
        get<1>(tree[state]) = beta;
        get<2>(tree[state]) = minEval.first;
        get<3>(tree[state]) = chosenAction;
        get<4>(tree[state]) = nodes;
        policyMap[state] = chosenAction;
        return minEval;
    }
}

pair<long long,long long> minimax(long long state, long long pastState, int depth, bool maximizingPlayer, int emptySlots)
{
    char player = maximizingPlayer ? 2 : 1;
    if (depth == 0 || emptySlots == 0) // or game is over in position
    {
        long long score = heuristic(state, pastState, player);
        get<2>(tree[state]) = score;
        get<3>(tree[state]) = state;
        return {score, state};
    }
    if (maximizingPlayer)
    {
        pair<long long, long long> maxEval, eval; // score, state;
        maxEval.first = INT_MIN;
        vector<long long> neighbors = getNeighbors(state, player);
        long long chosenAction;
        maxEval.first = INT_MIN;
        for(auto neighbor : neighbors)
        {
            eval = minimax(neighbor, state, depth - 1, false, emptySlots - 1);
            if(eval.first >= maxEval.first)
                chosenAction = neighbor;
            maxEval = (maxEval.first > eval.first) ? maxEval : eval;

            nodes[state].push_back({neighbor, eval.first});
        }
        get<2>(tree[state]) = maxEval.first;
        get<3>(tree[state]) = chosenAction;
        get<4>(tree[state]) = nodes;

        policyMap[state] = chosenAction;
        return maxEval;
    }
    else
    {
        pair<long long, long long> minEval, eval; // score, state;
        minEval.first = INT_MAX;
        vector<long long> neighbors = getNeighbors(state, !player);
        long long chosenAction;
        minEval.first = INT_MAX;
        for(auto neighbor : neighbors)
        {
            eval = minimax(neighbor, state, depth - 1, true, emptySlots - 1);
            if(eval.first <= minEval.first)
                chosenAction = neighbor;
            minEval = (minEval.first < eval.first) ? minEval : eval;
            
            nodes[state].push_back({neighbor, eval.first});
        }

        get<2>(tree[state]) = minEval.first;
        get<3>(tree[state]) = chosenAction;
        get<4>(tree[state]) = nodes;
        policyMap[state] = chosenAction;
        return minEval;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// end solving functions//////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// start main driver code/////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern "C"
{
    EMSCRIPTEN_KEEPALIVE
    void solve(int stat1,int stat2,int stat3,int stat4,int stat5, int pastStat1, int pastStat2, int pastStat3, int pastStat4,int pastStat5, int algorithmType, int depth, int emptySlots)
    {
        string result = "";

        // each state is send in five parts. So we have to reconstruct it.
        // start reconstruction of (state)
        string str = "";
        string s1 = to_string(stat1);
        while(s1.size() < 6)
        {
            s1 = "0" + s1;
        }
        string s2 = to_string(stat2);
        while(s2.size() < 9)
        {
            s2 = "0" + s2;
        }
        string s3 = to_string(stat3);
        while(s3.size() < 9)
        {
            s3 = "0" + s3;
        }
        string s4 = to_string(stat4);
        while(s4.size() < 9)
        {
            s4 = "0" + s4;
        }
        string s5 = to_string(stat5);
        while(s5.size() < 9)
        {
            s5 = "0" + s5;
        }
        str = s1 + s2 + s3 + s4 + s5;
        long long state = convertToBits(str);
        // end reconstruction of (state)
        // ***********************
        // start reconstruction of (pastState)
        s1 = to_string(pastStat1);
        while(s1.size() < 14)
        {
            s1 = "0" + s1;
        }
        s2 = to_string(pastStat2);
        while(s2.size() < 14)
        {
            s2 = "0" + s2;
        }
        s3 = to_string(pastStat3);
        while(s3.size() < 14)
        {
            s3 = "0" + s3;
        }
        s4 = to_string(pastStat4);
        while(s4.size() < 9)
        {
            s4 = "0" + s4;
        }
        s5 = to_string(pastStat5);
        while(s5.size() < 9)
        {
            s5 = "0" + s5;
        }
        str = s1 + s2 + s3 + s4 + s5;
        long long pastState = convertToBits(str);
        // end reconstruction of (pastState)
        
        if (algorithmType == 1)
        {
            minimax(state, pastState, depth, 1, emptySlots);
            result = stringify(state, algorithmType);
        }
        else
        {
            minimaxAB(state, pastState, depth, INT_MIN, INT_MAX, 1, emptySlots);
            result = stringify(state, algorithmType);
        }

        // result is ready to be send to javascript
        string script = "SolutionPanel('" + result + "')";
        const char *Schar = script.c_str();
        emscripten_run_script(Schar);
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// end main driver code///////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
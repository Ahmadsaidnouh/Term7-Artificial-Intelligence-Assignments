#include <bits/stdc++.h>

using namespace std;

#define ROWS 6
#define COLUMNS 7

#define AI_AGENT true
#define USER false

#define EMPTY 0
#define USER_SLOT 1
#define AI_AGENT_SLOT 2

#define SUB_SIZE 4

typedef vector<vector<int>> vector2D;

vector2D createBoard()
{
    vector2D board(ROWS, vector<int>(COLUMNS, 0));
    return board;
}

void fillSlot(vector2D &board, int row, int column, int slot)
{
    board[row][column] = slot;
}

bool validLocation(vector2D board, int column)
{
    return board[ROWS - 1][column] == 0;
}

int nextAvailableRow(vector2D board, int column)
{
    for (int row = 0; row < ROWS; row++)
    {
        if (board[row][column] == 0)
        {
            return row;
        }
    }

    return false;
}

void displayBoard(vector2D board)
{
    reverse(board.begin(), board.end());

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            cout << board[i][j];
            cout << " ";
        }
        cout << endl;
    }
    cout << endl;
}

bool winningMove(vector2D board, int slot)
{
    // 4 slots connect horizontally
    for (int column = 0; column < COLUMNS - 3; column++)
    {
        for (int row = 0; row < ROWS; row++)
        {
            if (board[row][column] == slot &&
                board[row][column + 1] == slot &&
                board[row][column + 2] == slot &&
                board[row][column + 3] == slot)
                return true;
        }
    }

    // 4 slots connect vertically
    for (int column = 0; column < COLUMNS; column++)
    {
        for (int row = 0; row < ROWS - 3; row++)
        {
            if (board[row][column] == slot &&
                board[row + 1][column] == slot &&
                board[row + 2][column] == slot &&
                board[row + 3][column] == slot)
                return true;
        }
    }

    // 4 slots connected in a positive slope manner
    for (int column = 0; column < COLUMNS - 3; column++)
    {
        for (int row = 0; row < ROWS - 3; row++)
        {
            if (board[row][column] == slot &&
                board[row + 1][column + 1] == slot &&
                board[row + 2][column + 2] == slot &&
                board[row + 3][column + 3] == slot)
                return true;
        }
    }

    // 4 slots connected in a negative slope manner
    for (int column = 0; column < COLUMNS - 3; column++)
    {
        for (int row = 3; row < ROWS; row++)
        {
            if (board[row][column] == slot &&
                board[row - 1][column + 1] == slot &&
                board[row - 2][column + 2] == slot &&
                board[row - 3][column + 3] == slot)
                return true;
        }
    }

    return false;
}

int doEvaluation(vector<int> subVector, int slot)
{
    int score = 0;
    int computer_slot = USER_SLOT;

    if (slot == USER_SLOT)
    {
        computer_slot = AI_AGENT_SLOT;
    }

    if (count(subVector.begin(), subVector.end(), slot) == 4)
        score += 100;
    else if (count(subVector.begin(), subVector.end(), slot) == 3 &&
             count(subVector.begin(), subVector.end(), EMPTY) == 1)
        score += 5;
    else if (count(subVector.begin(), subVector.end(), slot) == 2 &&
             count(subVector.begin(), subVector.end(), EMPTY) == 2)
        score += 2;
    if (count(subVector.begin(), subVector.end(), computer_slot) == 3 &&
        count(subVector.begin(), subVector.end(), EMPTY) == 1)
        score -= 4;

    return score;
}

int scorePosition(vector2D board, int slot)
{
    int score = 0;

    // score in the center column
    vector<int> centerVector;
    for (int i = 0; i < ROWS; i++)
    {
        centerVector.push_back(board[i][COLUMNS / 2]);
    }

    int centerCount = count(centerVector.begin(), centerVector.end(), slot);
    score += centerCount * 3;

    // score in the horizontal rows
    for (int i = 0; i < ROWS; i++)
    {
        vector<int> rowVector;
        for (int j = 0; j < COLUMNS; j++)
        {
            rowVector.push_back(board[i][j]);
        }
        for (int column = 0; column < COLUMNS - 3; column++)
        {
            vector<int> subVector;
            for (int k = column; k < column + SUB_SIZE; k++)
            {
                subVector.push_back(rowVector[k]);
            }

            score += doEvaluation(subVector, slot);
        }
    }

    // score in the vertical columns
    for (int i = 0; i < COLUMNS; i++)
    {
        vector<int> columnVector;
        for (int j = 0; j < ROWS; j++)
        {
            columnVector.push_back(board[j][i]);
        }
        for (int k = 0; k < ROWS - 3; k++)
        {
            vector<int> subVector;
            for (int row = k; row < k + SUB_SIZE; row++)
            {
                subVector.push_back(columnVector[row]);
            }
            score += doEvaluation(subVector, slot);
        }
    }

    // score in the diagonal with positive slope
    for (int row = 0; row < ROWS - 3; row++)
    {
        for (int column = 0; column < COLUMNS - 3; column++)
        {
            vector<int> subVector;
            for (int k = 0; k < SUB_SIZE; k++)
            {
                subVector.push_back(board[row + k][column + k]);
            }
            score += doEvaluation(subVector, slot);
        }
    }

    // score in the diagonal with negative slope
    for (int row = 0; row < ROWS - 3; row++)
    {
        for (int column = 0; column < COLUMNS - 3; column++)
        {
            vector<int> subVector;
            for (int k = 0; k < SUB_SIZE; k++)
            {
                subVector.push_back(board[row + 3 - k][column + k]);
            }
            score += doEvaluation(subVector, slot);
        }
    }

    return score;
}

vector<int> getAvailableLocations(vector2D board)
{
    vector<int> availableLocations;
    for (int column = 0; column < COLUMNS; column++)
    {
        if (validLocation(board, column))
        {
            availableLocations.push_back(column);
        }
    }

    return availableLocations;
}

bool checkTerminalNode(vector2D board)
{
    return winningMove(board, USER_SLOT) ||
           winningMove(board, AI_AGENT_SLOT) ||
           getAvailableLocations(board).size() == 0;
}

int getBestMove(vector2D board, int slot)
{
    srand(time(0));
    vector<int> availableLocations = getAvailableLocations(board);
    int bestScore = numeric_limits<int>::min();
    int bestColumnIndex = rand() % availableLocations.size();
    int bestColumn = availableLocations[bestColumnIndex];
    for (int column = 0; column < availableLocations.size(); column++)
    {
        int row = nextAvailableRow(board, column);
        vector2D tempBoard;
        tempBoard.insert(tempBoard.begin(), board.begin(), board.end());
        fillSlot(tempBoard, row, column, slot);
        int score = scorePosition(tempBoard, slot);
        if (score > bestScore)
        {
            bestScore = score;
            bestColumn = column;
        }
    }

    return bestColumn;
}

pair<int, int> minimax(vector2D board, int depth, int alpha, int beta, bool maximizer)
{
    vector<int> availableLocations = getAvailableLocations(board);
    bool isTerminal = checkTerminalNode(board);
    if (depth == 0 || isTerminal)
    {
        if (isTerminal)
        {
            if (winningMove(board, AI_AGENT_SLOT))
            {
                return make_pair(NULL, numeric_limits<int>::max());
            }
            else if (winningMove(board, USER_SLOT))
            {
                return make_pair(NULL, numeric_limits<int>::min());
            }
            else
            {
                return make_pair(NULL, 0);
            }
        }
        else
        {
            return make_pair(NULL, scorePosition(board, AI_AGENT));
        }
    }

    int value;
    int index;
    int column;
    int row;
    vector2D boardImage;
    int newScore;

    if (maximizer)
    {
        value = numeric_limits<int>::min();
        index = rand() % availableLocations.size();
        column = availableLocations[index];
        for (int col = 0; col < availableLocations.size(); col++)
        {
            row = nextAvailableRow(board, availableLocations[col]);
            boardImage.insert(boardImage.begin(), board.begin(), board.end());
            fillSlot(boardImage, row, availableLocations[col], AI_AGENT_SLOT);
            newScore = minimax(boardImage, depth - 1, alpha, beta, false).second;
            if (newScore > value)
            {
                value = newScore;
                column = availableLocations[col];
            }
            alpha = max(alpha, value);
            if (alpha >= beta)
            {
                break;
            }
        }

        return make_pair(column, value);
    }

    else
    {
        value = numeric_limits<int>::max();
        index = rand() % availableLocations.size();
        column = availableLocations[index];
        for (int col = 0; col < availableLocations.size(); col++)
        {
            row = nextAvailableRow(board, availableLocations[col]);
            boardImage.insert(boardImage.begin(), board.begin(), board.end());
            fillSlot(boardImage, row, availableLocations[col], USER_SLOT);
            newScore = minimax(boardImage, depth - 1, alpha, beta, true).second;
            if (newScore < value)
            {
                value = newScore;
                column = availableLocations[col];
            }

            beta = min(beta, value);
            if (alpha >= beta)
                break;
        }

        return make_pair(column, value);
    }
}

int main()
{
    vector2D gameBoard = createBoard();
    displayBoard(gameBoard);
    bool gameOver = false;
    int turn = rand() % 2;

    while (!gameOver)
    {
        if (turn == USER)
        {
            int c = 0;
            cin >> c;
            if (validLocation(gameBoard, c))
            {
                int row = nextAvailableRow(gameBoard, c);
                fillSlot(gameBoard, row, c, USER_SLOT);
                displayBoard(gameBoard);
                if (winningMove(gameBoard, USER_SLOT))
                {
                    cout << "PLAYER WON" << endl;
                    break;
                }

                turn += 1;
                turn %= 2;
            }
        }
        else
        {
            pair<int, int> p;
            p = minimax(gameBoard, 5, numeric_limits<int>::min(), numeric_limits<int>::max(), true);
            if (validLocation(gameBoard, p.first))
            {
                int row = nextAvailableRow(gameBoard, p.first);
                fillSlot(gameBoard, row, p.first, AI_AGENT_SLOT);
                displayBoard(gameBoard);
                if (winningMove(gameBoard, AI_AGENT))
                {
                    cout << "AI_AGENT WON" << endl;
                    break;
                }

                turn += 1;
                turn %= 2;
            }
        }
    }
    return 0;
}
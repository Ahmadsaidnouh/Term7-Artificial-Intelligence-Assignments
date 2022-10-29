#include <vector>
#include <string>
#include <set>
#include <unordered_map>

using namespace std;

typedef vector<vector<int>> vector2d;
typedef unordered_map<string, string> uMap;
typedef pair<int, string> frontierPair;

void setGrid(unordered_map<int, pair<int, int>> &positions, string goalState);

pair<int, int> stringToVectorPositions(string);

pair<int, int> findZero(string);

vector2d toVector2D(string);

bool isGoal(string, string);

void print2dVector(vector2d);

int calculateHeuristic(string state, int type);

pair<string, pair<int, int>> checkDirections(string);

vector<string> findNeighbors(string state);

vector<int> tracePath(uMap, string);

bool setSearch(set<string> explored, string state);

string toString(vector2d state);
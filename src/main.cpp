//#include "gurobi_c++.h"
#include "parser.hpp"
#include <cstring>
#include <iostream>
using namespace std;

int main(int argc,
    char* argv[])
{
    cout << "Hello World!" << endl;

    vector<vector<int>> map = parseMap("../../../data/map.ppm");

    for (int y = 0; y < map.size(); y++)
    {
        for (int x = 0; x < map[y].size(); x++)
        {
            cout << map[y][x];
        }
        cout << endl;
    }

    return 0;
}
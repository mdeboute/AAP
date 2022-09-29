#include "parser.hpp"
#include "filesystem.hpp"
#include "mip_formulation.hpp"
#include "utils.hpp"
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    const fs::path root_dir = fs::current_path().parent_path();
    const string data_dir = string(root_dir) + "/data";
    // const string data_dir = "../data";

    string map_file = data_dir + "/map.ppm";
    string config_file = data_dir + "/config.txt";

    vector<float> config = parseConfig(config_file);
    vector<vector<int>> map = parseMap(map_file);

    cout << "Number of angles: " << config[0] << endl;
    cout << "Furnace radius: " << config[1] << endl;
    cout << "Radius of action of a firefighter: " << config[2] << endl;
    cout << endl;
    cout << "Map size: " << map.size() << "x" << map[0].size() << endl;

    display_map(map);

    // map = solve(map, config);

    return 0;
}
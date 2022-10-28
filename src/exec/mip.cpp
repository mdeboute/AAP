#include "file_io.hpp"
#include "mip_formulation.hpp"
#include "utils.hpp"
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Usage: ./mip.out <input_dir>" << endl;
        return 1;
    }
    const string data_dir = argv[1];
    const string map_file = data_dir + "/map.ppm";
    const string config_file = data_dir + "/config.txt";

    vector<float> config = parse_config(config_file);
    vector<vector<Color>> map = parse_map(map_file);

    cout << "Number of angles: " << config[0] << endl;
    cout << "Furnace radius: " << config[1] << endl;
    cout << "Radius of action of a firefighter: " << config[2] << endl;
    cout << "Map size: " << map.size() << "x" << map[0].size() << endl;
    cout << endl;

    Graph graph = calculate_graph_data(map, config, false, false);

    vector<FighterVertex> solution = mip_solve(graph);

    vector<string> splitString = split_string(data_dir, "/");
    if (splitString[splitString.size() - 1].empty())
        splitString.erase(splitString.end()-1);
    const string result_file = "../solution/result_" + splitString[splitString.size() -1] + ".ppm";
    cout << "Writing result to " << result_file << endl;
    write_solution(result_file, map, config, solution);

    return 0;
}
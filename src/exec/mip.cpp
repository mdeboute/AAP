#include "Graph/FighterVertex.hpp"
#include "Graph/FireVertex.hpp"
#include "utils.hpp"
#include "file_io.hpp"
#include "mip_formulation.hpp"

#include <vector>
#include <cstring>

using namespace std;

const string MAP_FILE = "/map.ppm";
const string CONFIG_FILE = "/config.txt";

void display_usage()
{
    cout << "Usage: ./mip <instance_directory>" << endl;
}

void display_solution(const vector<FighterVertex> solution)
{
    for (FighterVertex fighter : solution)
    {
        cout << "We place a fighter at position (" << fighter.getPos().getX() << ", " << fighter.getPos().getY() << ")" << endl;
    }
}

void display_data(vector<float> config, vector<vector<Color>> map)
{
    cout << "Number of angles: " << config[0] << endl;
    cout << "Furnace radius: " << config[1] << endl;
    cout << "Radius of action of a firefighter: " << config[2] << endl;
    cout << "Map size: " << map.size() << "x" << map[0].size() << endl;
    cout << endl;
}

const string get_result_file(const string data_dir)
{
    vector<string> splitString = split_string(data_dir, "/");
    if (splitString[splitString.size() - 1].empty())
        splitString.erase(splitString.end() - 1);
    const string result_file = "../solution/result_" + splitString[splitString.size() - 1] + ".ppm";
    cout << "\nWriting result to " << result_file << endl;
    return result_file;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        display_usage();
        return 1;
    }

    const string data_dir = argv[1];
    const string map_file = data_dir + MAP_FILE;
    const string config_file = data_dir + CONFIG_FILE;

    vector<float> config = parse_config(config_file);
    vector<vector<Color>> map = parse_map(map_file);

    display_data(config, map);

    Graph graph = calculate_graph_data(map, config, false, false);

    vector<FighterVertex> bestTeam = mip_solve(graph);
    display_solution(bestTeam);
    const string result_file = get_result_file(data_dir);
    write_solution(result_file, map, config, bestTeam);

    return 0;
}
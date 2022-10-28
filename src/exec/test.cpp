#include "Graph/FighterVertex.hpp"
#include "Graph/FireVertex.hpp"
#include "utils.hpp"
#include "brute_force.hpp"
#include "greedy.hpp"

#include <vector>
#include <cstring>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Usage: ./test.out <instance_dir> <algo>" << endl;
        cout << "Algo: -b for brute force, -g for greedy" << endl;
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

    Graph graph = calculate_graph_data(map, config, true, true);

    vector<string> splitString = split_string(data_dir, "/");
    if (splitString[splitString.size() - 1].empty())
        splitString.erase(splitString.end()-1);
    const string result_file = "../solution/result_" + splitString[splitString.size() - 1] + ".ppm";

    if (strcmp(argv[2], "-b") == 0)
    {
        vector<FighterVertex> bestTeam = bruteforce_solve(graph);
        for (FighterVertex fighter : bestTeam)
        {
            cout << "We place a fighter at position (" << fighter.getPos().getX() << ", " << fighter.getPos().getY() << ")" << endl;
        }
        cout << "\nWriting result to " << result_file << endl;
        write_solution(result_file, map, config, bestTeam);
    }
    else if (strcmp(argv[2], "-g") == 0)
    {
        vector<FighterVertex> bestTeam = greedy_solve(graph);
        for (FighterVertex fighter : bestTeam)
        {
            cout << "We place a fighter at position (" << fighter.getPos().getX() << ", " << fighter.getPos().getY() << ")" << endl;
        }
        cout << "\nWriting result to " << result_file << endl;
        write_solution(result_file, map, config, bestTeam);
    }
    else
    {
        cout << "Unknown algorithm!" << endl;
        return 1;
    }
    return 0;
}
#include "Graph/FighterVertex.hpp"
#include "Graph/FireVertex.hpp"
#include "utils.hpp"
#include "brute_force.hpp"

#include <vector>

using namespace std;

int main()
{
    Position pos_1(0, 0);
    Position pos_2(1, 0);
    Position pos_3(2, 0);
    Position pos_4(0, 1);
    Position pos_5(0, 2);
    Position pos_6(1, 1);
    Position pos_7(1, 2);
    Position pos_8(2, 1);
    Position pos_9(2, 2);

    FireVertex fire_1(pos_1, pos_9, 1);
    FireVertex fire_2(pos_5, pos_9, 2);

    FighterVertex fighter_1(pos_6, 1);
    FighterVertex fighter_2(pos_7, 2);

    fighter_1.addFire(fire_1);
    fighter_2.addFire(fire_2);

    vector<FighterVertex> fighters;
    fighters.push_back(fighter_1);
    fighters.push_back(fighter_2);

    vector<FireVertex> fires;
    fires.push_back(fire_1);
    fires.push_back(fire_2);

    // fighters = cutUselessFighters(fighters);

    vector<vector<FighterVertex>> partitions = findPartitions(fighters);

    // for (int i = 0; i < partitions.size(); i++)
    // {
    //     cout << "Partition " << i << " : ";
    //     for (int j = 0; j < partitions[i].size(); j++)
    //     {
    //         cout << partitions[i][j] << " ; ";
    //     }
    //     cout << endl;
    // }

    // check the faisability of the partitions
    // int cpt = 0;
    // for (vector<FighterVertex> fighters : partitions)
    // {
    //     cpt++;
    //     cout << "Partition " << cpt << ": " << checker(fighters, fires) << endl;
    // }

    vector<FighterVertex> bestTeam = solve(partitions, fires);

    cout << "Best team:\n";
    for (FighterVertex fighter : bestTeam)
    {
        cout << fighter << endl;
    }
}
#include "brute_force.hpp"
#include <vector>
#include "Graph/FighterVertex.hpp"
#include <cmath>

std::vector<std::vector<FighterVertex>> findPartitions(std::vector<FighterVertex> fighterList)
{
    int n = fighterList.size();
    std::vector<std::vector<FighterVertex>> partitions;

    for (int i = 0; i < pow(2, n); i++)
    {
        std::vector<FighterVertex> partition;
        for (int j = 0; j < n; j++)
        {
            if (i & (1 << j))
            {
                partition.push_back(fighterList[j]);
            }
        }
        partitions.push_back(partition);
    }
    return partitions;
}
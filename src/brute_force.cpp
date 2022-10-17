#include "brute_force.hpp"
#include <vector>
#include <bitset>
#include "Graph/FighterVertex.hpp"
#include <cmath>

int compute_lower_bound(std::vector<FighterVertex> fighterList, std::vector<FireVertex> fireList)
{
    int max = 0;
    for (FighterVertex fighter : fighterList)
    {
        int capacity = fighter.getFireCapacity();
        if (capacity > max)
        {
            max = capacity;
        }
    }
    return ceil((double)fireList.size() / max);
}

bool check_feasibility(std::vector<FighterVertex> fighters, std::vector<FireVertex> fires)
{
    for (FireVertex fire : fires)
    {
        bool find = false;
        for (FighterVertex fighter : fighters)
        {
            if (fighter.stopFire(fire))
            {
                find = true;
                break;
            }
        }
        if (!find)
        {
            return false;
        }
    }
    return true;
}

std::vector<std::vector<FighterVertex>> find_partitions(std::vector<FighterVertex> fighters, std::vector<FireVertex> fires)
{
    int n = fighters.size();
    int upperBound = fires.size();
    int lowerBound = compute_lower_bound(fighters, fires);

    std::vector<std::vector<FighterVertex>> partitions;

    for (int i = 0; i < std::pow(2, n); i++)
    {
        std::vector<FighterVertex> subset;

        // we want the subset of size at most upperBound and at least lowerBound that are feasible (using the checker)
        if (std::bitset<32>(i).count() <= upperBound && std::bitset<32>(i).count() >= lowerBound)
        {
            for (int j = 0; j < n; j++)
            {
                if (i & (1 << j))
                {
                    subset.push_back(fighters[j]);
                }
            }
            if (check_feasibility(subset, fires))
            {
                partitions.push_back(subset);
            }
        }
    }
    return partitions;
}

std::vector<FighterVertex> solve(std::vector<std::vector<FighterVertex>> partitions, std::vector<FireVertex> fires)
{
    std::vector<FighterVertex> bestTeam;
    int bestSize = fires.size();
    for (std::vector<FighterVertex> team : partitions)
    {
        if (team.size() <= bestSize)
        {
            bestTeam = team;
            bestSize = team.size();
        }
    }
    return bestTeam;
}

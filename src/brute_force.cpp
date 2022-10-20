#include "brute_force.hpp"
#include <vector>
#include <bitset>
#include "Graph/FighterVertex.hpp"
#include <chrono>
#include <cmath>

int compute_lower_bound(const std::vector<FighterVertex> &fighterList, const std::vector<FireVertex> &fireList)
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

bool check_feasibility(const std::vector<FighterVertex> &fighters, const std::vector<FireVertex> &fires)
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

std::vector<FighterVertex> bruteforce_solve(const Graph &graph)
{
    std::vector<FighterVertex> fighters = graph.getFigtherVertexTab();
    std::vector<FireVertex> fires = graph.getFireVertexTab();

    int n = fighters.size();
    int upperBound = fires.size();
    int lowerBound = compute_lower_bound(fighters, fires);

    for (int i = 0; i < std::pow(2, n); i++)
    {
        std::vector<FighterVertex> team;

        // we want the team of size at most upperBound and at least lowerBound that are feasible (using the checker)
        if (std::bitset<32>(i).count() <= upperBound && std::bitset<32>(i).count() >= lowerBound)
        {
            for (int j = 0; j < n; j++)
            {
                if (i & (1 << j))
                {
                    team.push_back(fighters[j]);
                }
            }
            if (check_feasibility(team, fires))
            {
                return team;
            }
        }
    }
    std::cout << "No solution found" << std::endl;
    return std::vector<FighterVertex>();
}

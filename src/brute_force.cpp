#include "brute_force.hpp"
#include <algorithm>
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

bool check_feasibility(const std::vector<FighterVertex> &fighterList, const std::vector<FireVertex> &fireList)
{
    for (FireVertex fire : fireList)
    {
        bool find = false;
        for (FighterVertex fighter : fighterList)
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
    std::vector<FighterVertex> fighters = graph.getFigtherVertexList();
    std::vector<FireVertex> fires = graph.getFireVertexList();

    auto startingTime = std::chrono::steady_clock::now();

    int n = fighters.size();
    int upperBound = fires.size();
    int lowerBound = compute_lower_bound(fighters, fires);

    std::cout << "Number of potential fighters: " << n << std::endl;
    std::cout << std::endl;

    // sort the fighters list by the number of fires they can stop (we earn ~10% of the time)
    std::sort(fighters.begin(), fighters.end(), [](FighterVertex &a, FighterVertex &b)
              { return a.getFireCapacity() > b.getFireCapacity(); });

    std::vector<FighterVertex> bestSolution;
    int bestSize = upperBound;

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
            if (check_feasibility(team, fires) && team.size() < bestSize)
            {
                bestSolution = team;
                bestSize = team.size();
            }
        }
    }

    std::chrono::duration<double> tt = std::chrono::steady_clock::now() - startingTime;
    std::cout << "Result: runtime = " << tt.count() << " sec; objective value = " << bestSize << std::endl;
    std ::cout << std::endl;
    return bestSolution;
}

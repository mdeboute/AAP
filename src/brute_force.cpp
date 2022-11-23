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


int dichotomic_search(const std::vector<int>& list, int start, int end, int val) {
    if (end - start <= 0)
        return 0;
    int middle = (int)((end + start) / 2);
    if (list[middle] == val)
        return -1;
    if (list[middle] < val)
        return dichotomic_search(list, middle + 1, end, val);
    return dichotomic_search(list, start, middle, val);
}

bool insertID(const FireVertex& fire, std::vector<int>& fireIdList) {
    int pos = dichotomic_search(fireIdList, 0, fireIdList.size(), fire.getID());
    if (pos == -1)
        return false;
    fireIdList.insert(fireIdList.begin() + pos, fire.getID());
    return true;
}

bool check_feasibility(const std::vector<FighterVertex>& fighterList, const std::vector<FireVertex>& fireList)
{
    std::vector<int> coveredFiresIds;
    for (FighterVertex fighter : fighterList)
    {
        for (const FireVertex& fire : fighter.getFireCovered())
        {
            bool success = insertID(fire, coveredFiresIds);
            if (success && coveredFiresIds.size() == fireList.size())
                return true;
        }
    }
    return false;
}

std::vector<FighterVertex> bruteforce_solve(const Graph &graph)
{
    std::vector<FighterVertex> fighters = graph.getFigtherVertexList();
    std::vector<FireVertex> fires = graph.getFireVertexList();

    auto startingTime = std::chrono::steady_clock::now();

    int n = fighters.size();
    int upperBound = fires.size();
    int lowerBound = compute_lower_bound(fighters, fires);

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

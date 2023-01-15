#include "brute_force.hpp"
#include "Graph/FighterVertex.hpp"
#include "utils.hpp"
#include <algorithm>
#include <vector>
#include <bitset>
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

std::vector<FighterVertex> bruteforce_solve(const Graph &graph)
{
    std::vector<FighterVertex> fighters = graph.getFigtherVertexList();
    std::vector<FireVertex> fires = graph.getFireVertexList();

    auto startingTime = std::chrono::steady_clock::now();

    int n = fighters.size();

    std::cout << "Number of potential fighters: " << n << std::endl;

    int upperBound = n;
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
    std::cout << std::endl;
    return bestSolution;
}

void next(std::vector<bool> &partition)
{
    int listSize = partition.size();
    for (int i = 1; i < listSize; ++i)
    {
        if (partition[listSize - 1 - i] == 1 && partition[listSize - i] == 0)
        {
            partition[listSize - 1 - i] = 0;
            partition[listSize - i] = 1;
            int tmp = 0;
            for (int j = listSize - i + 1; j < listSize; ++j)
            {
                if (partition[j] == 1)
                {
                    tmp++;
                    partition[j] = 0;
                    partition[listSize - i + tmp] = 1;
                }
            }
            break;
        }
    }
}

std::vector<std::vector<bool>> generate_partitions(int n, int k)
{
    std::vector<std::vector<bool>> partitions;
    std::vector<bool> partition(n, 0);
    for (int i = 0; i < k; ++i)
    {
        partition[i] = 1;
    }
    partitions.push_back(partition);
    double np = std::tgamma(n + 1) / (tgamma(k + 1) * tgamma(n - k + 1));
    for (int i = 0; i < np; ++i)
    {
        next(partition);
        partitions.push_back(partition);
    }
    return partitions;
}

std::vector<FighterVertex> better_bruteforce_solve(const Graph &graph)
{
    std::vector<FighterVertex> fighters = graph.getFigtherVertexList();
    std::vector<FireVertex> fires = graph.getFireVertexList();

    auto startingTime = std::chrono::steady_clock::now();

    int n = fighters.size();

    std::cout << "Number of potential fighters: " << n << std::endl;

    int upperBound = n;
    int lowerBound = compute_lower_bound(fighters, fires);

    std::sort(fighters.begin(), fighters.end(), [](FighterVertex &a, FighterVertex &b)
              { return a.getFireCapacity() > b.getFireCapacity(); });

    std::vector<FighterVertex> bestSolution;
    int solFound = 0;

    while (solFound == 0)
    {
        // std::cout << "Bound at " << lowerBound << std::endl;
        std::vector<std::vector<bool>> partitions = generate_partitions(n, lowerBound);

        for (std::vector<bool> partition : partitions)
        {
            std::vector<FighterVertex> potentialSol;
            for (int i = 0; i < partition.size(); ++i)
            {
                if (partition[i] == 1)
                {
                    potentialSol.push_back(fighters[i]);
                }
            }
            if (check_feasibility(potentialSol, fires))
            {
                std::chrono::duration<double> tt = std::chrono::steady_clock::now() - startingTime;
                std::cout << "Result: runtime = " << tt.count() << " sec; objective value = " << potentialSol.size() << std::endl;
                std ::cout << std::endl;
                return potentialSol;
            }
        }

        lowerBound++;
    }
    std::cout << "No solution found!" << std::endl;
    std::cout << std::endl;
    return std::vector<FighterVertex>();
}
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
        int capacity = fighter.getNbFireCovered();
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
              { return a.getNbFireCovered() > b.getNbFireCovered(); });

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

void generate_partitions(int n, int k, int i, int nb_selected, std::vector<bool> &partition, std::vector<std::vector<bool>> &partitions)
{
    if (i + k - nb_selected > n)
        return;
    if (nb_selected == k || i == n)
    {
        std::vector<bool> complete_partition = partition;
        partitions.push_back(complete_partition);
        return;
    }
    partition.push_back(1);
    generate_partitions(n, k, i + 1, nb_selected + 1, partition, partitions);
    partition.pop_back();
    partition.push_back(0);
    generate_partitions(n, k, i + 1, nb_selected, partition, partitions);
    partition.pop_back();
    return;
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
              { return a.getNbFireCovered() > b.getNbFireCovered(); });

    std::vector<FighterVertex> bestSolution;
    int solFound = 0;

    while (solFound == 0)
    {
        std::vector<std::vector<bool>> partitions;
        std::vector<bool> partition;
        generate_partitions(n, lowerBound, 0, 0, partition, partitions);
        // std::cout << "Bound at " << lowerBound << " with " << partitions.size() << " partitions" << std::endl;

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
#include "greedy.hpp"
#include <chrono>

std::vector<FighterVertex> greedy_solve(const Graph &graph, bool verbose)
{
    std::vector<FighterVertex> fighters = graph.getFigtherVertexList();
    if (fighters.size() == 0)
        std::cout << "Invalid input in greedy!" << std::endl;
    std::vector<FireVertex> fires = graph.getFireVertexList();

    auto startingTime = std::chrono::steady_clock::now();

    std::vector<FighterVertex> sol;

    std::vector<int> isFireCovered;
    std::vector<int> fightersValue;
    for (int i = 0; i < fires.size(); ++i)
    {
        isFireCovered.push_back(0);
    }
    for (int i = 0; i < fighters.size(); ++i)
    {
        fightersValue.push_back(0);
    }

    int allFireCovered = 0;
    while (allFireCovered == 0)
    {
        allFireCovered = 1;

        // compute value of fighters
        for (int i = 0; i < fightersValue.size(); ++i)
        {
            if (fightersValue[i] >= 0)
            {
                fightersValue[i] = 0;
                std::vector<FireVertex> fighterFires = fighters[i].getFireCovered();
                for (int j = 0; j < fighterFires.size(); ++j)
                {
                    if (isFireCovered[fighterFires[j].getIndex()] == 0)
                    {
                        fightersValue[i]++;
                    }
                }
            }
        }

        int bestFighter = -1;
        int bestValue = -1;
        for (int i = 0; i < fightersValue.size(); i++)
        {
            if (fightersValue[i] > bestValue)
            {
                bestValue = fightersValue[i];
                bestFighter = i;
            }
        }

        fightersValue[bestFighter] = -1;
        sol.push_back(fighters[bestFighter]);
        std::vector<FireVertex> fighterFires = fighters[bestFighter].getFireCovered();
        for (int j = 0; j < fighterFires.size(); ++j)
        {
            isFireCovered[fighterFires[j].getIndex()] = 1;
        }

        for (int i = 0; i < fires.size(); ++i)
        {
            if (allFireCovered == 1)
            {
                if (isFireCovered[i] == 0)
                {
                    allFireCovered = 0;
                }
            }
        }
    }
    std::chrono::duration<double> tt = std::chrono::steady_clock::now() - startingTime;
    if (verbose)
    {
        std::cout << "Result: runtime = " << tt.count() << " sec; objective value = " << sol.size() << std::endl;
        std::cout << std::endl;
    }
    return sol;
}
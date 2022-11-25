#include "genetic_search.hpp"
#include "greedy.hpp"
#include <numeric>
#include <chrono>

Fellow generate_random_fellow(int nbChromosomes)
{
    Fellow fellow;
    for (int i = 0; i < nbChromosomes; i++)
    {
        fellow.push_back(rand() % 2);
    }
    return fellow;
}

Solution decode_fellow(const Fellow &fellow, const Data &data)
{
    Solution solution;
    for (int i = 0; i < fellow.size(); i++)
    {
        if (fellow[i] == 1)
        {
            solution.push_back(data.getFigtherVertexList()[i]);
        }
    }
    return solution;
}

Fellow decode_solution(const Solution &solution, const Data &data)
{
    Fellow fellow;
    for (int i = 0; i < data.getFigtherVertexList().size(); i++)
    {
        fellow.push_back(0);
    }
    for (int i = 0; i < solution.size(); i++)
    {
        for (int j = 0; j < data.getFigtherVertexList().size(); j++)
        {
            if (solution[i] == data.getFigtherVertexList()[j])
            {
                fellow[j] = 1;
            }
        }
    }
    return fellow;
}

int fitness(const Fellow &fellow, const Data &data)
{
    Solution solution = decode_fellow(fellow, data);
    if (check_feasibility(solution, data.getFireVertexList()))
    {
        return solution.size();
    }
    return __INT_MAX__;
}

Fellow selection(const Population &population, std::vector<int> &scores, int k)
{
    // select a random fellow from the population and took his index
    int index = rand() % population.size();
    // select a random subset of index of size k from the population
    std::vector<int> subset;
    for (int i = 0; i < k; i++)
    {
        subset.push_back(rand() % population.size());
    }
    // for each index i in the subset, check if the score of the index index is better than the current one (<)
    for (int i = 0; i < subset.size(); i++)
    {
        if (scores[subset[i]] < scores[index])
        {
            index = subset[i];
        }
    }
    return population[index];
}

void mutate(Fellow fellow, float mutationRate)
{
    for (int i = 0; i < fellow.size(); i++)
    {
        if (rand() % 100 < mutationRate * 100)
        {
            fellow[i] = !fellow[i];
        }
    }
}

Population crossover(const Fellow &guy1, const Fellow &guy2, float crossoverRate)
{
    Population population;
    Fellow child1 = guy1;
    Fellow child2 = guy2;
    for (int i = 0; i < guy1.size(); i++)
    {
        if (rand() % 100 < crossoverRate * 100)
        {
            child1[i] = guy2[i];
            child2[i] = guy1[i];
        }
    }
    population.push_back(child1);
    population.push_back(child2);
    return population;
}

Solution genetic_solve(const Data &data, int populationSize, int nbGenerations, float mutationRate, float crossoverRate, int k)
{
    // std::vector<FighterVertex> fighters = data.getFigtherVertexList();
    // int nbChromosomes = fighters.size();
    auto startingTime = std::chrono::steady_clock::now();
    // generate a random population of size populationSize
    Fellow gigaChad = decode_solution(greedy_solve(data), data);
    Population population;
    for (int i = 0; i < populationSize; i++)
    {
        mutate(gigaChad, mutationRate);
        population.push_back(gigaChad);
    }
    Fellow bestGuy;
    int bestScore = __INT_MAX__;
    // for each generation
    for (int i = 0; i < nbGenerations; i++)
    {
        // compute the score of each fellow in the population
        std::vector<int> scores;
        for (int j = 0; j < population.size(); j++)
        {
            scores.push_back(fitness(population[j], data));
        }
        // check for new best solution
        for (int j = 0; j < scores.size(); j++)
        {
            if (scores[j] < bestScore)
            {
                bestScore = scores[j];
                bestGuy = population[j];
            }
        }
        // select parents
        Population parents;
        for (int j = 0; j < populationSize; j++)
        {
            parents.push_back(selection(population, scores, k));
        }
        // create the new generation
        Population newPopulation;
        for (int j = 0; j < populationSize; j += 2)
        {
            // get selected parents in pairs
            Fellow parent1 = parents[j];
            Fellow parent2 = parents[j + 1];
            // crossover and mutation
            Population childrens = crossover(parent1, parent2, crossoverRate);
            for (Fellow child : childrens)
            {
                mutate(child, mutationRate);
                newPopulation.push_back(child);
            }
        }
        population = newPopulation;
    }
    std::chrono::duration<double> tt = std::chrono::steady_clock::now() - startingTime;
    std::cout << "Result: runtime = " << tt.count() << " sec; objective value = " << fitness(bestGuy, data) << std::endl;
    std ::cout << std::endl;
    return decode_fellow(bestGuy, data);
}

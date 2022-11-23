#include "genetic_search.hpp"
#include <numeric>

Guy generate_random_guy(int nbChromosomes)
{
    Guy guy;
    for (int i = 0; i < nbChromosomes; i++)
    {
        guy.push_back(rand() % 2);
    }
    return guy;
}

Solution decode_guy(const Guy &guy, const Data &data)
{
    Solution solution;
    for (int i = 0; i < guy.size(); i++)
    {
        if (guy[i])
        {
            solution.push_back(data.getFigtherVertexList()[i]);
        }
    }
    return solution;
}

int fitness(const Guy &guy, const Data &data)
{
    Solution solution = decode_guy(guy, data);
    if (check_feasibility(solution, data.getFireVertexList()))
    {
        // sum the elements of the vector guy
        return std::accumulate(guy.begin(), guy.end(), 0);
    }
    return __INT_MAX__;
}

Guy selection(const Population &population, std::vector<int> &scores, int k)
{
    // select a random guy from the population and took his index
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

void mutate(Guy guy, float mutation_rate)
{
    for (int i = 0; i < guy.size(); i++)
    {
        if (rand() % 100 < mutation_rate * 100)
        {
            guy[i] = !guy[i];
        }
    }
}

Population crossover(const Guy &guy1, const Guy &guy2, float crossoverRate)
{
    Population population;
    Guy child1 = guy1;
    Guy child2 = guy2;
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
    // generate a random population of size populationSize
    Population population;
    for (int i = 0; i < populationSize; i++)
    {
        population.push_back(generate_random_guy(data.getFigtherVertexList().size()));
    }
    Guy bestGuy;
    int bestScore = __INT_MAX__;
    // for each generation
    for (int i = 0; i < nbGenerations; i++)
    {
        // compute the score of each guy in the population
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
                std::cout << "New best score: " << bestScore << std::endl;
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
            Guy parent1 = parents[j];
            Guy parent2 = parents[j + 1];
            // crossover and mutation
            Population childrens = crossover(parent1, parent2, crossoverRate);
            for (Guy child : childrens)
            {
                mutate(child, mutationRate);
                newPopulation.push_back(child);
            }
        }
        population = newPopulation;
    }
    return decode_guy(bestGuy, data);
}

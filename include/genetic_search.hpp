#ifndef GENETIC_SEARCH_HPP
#define GENETIC_SEARCH_HPP

#include <vector>
#include "Graph/Graph.hpp"
#include "utils.hpp"

using Solution = std::vector<FighterVertex>;
using Guy = std::vector<bool>;
using Population = std::vector<Guy>;
using Data = Graph;

Guy generate_random_guy(int nbChromosomes);
Solution decode_guy(const Guy &guy);
int fitness(const Guy &guy, const Data &data);
Guy selection(const Population &population, std::vector<int> &scores, int k);
void mutation(Guy guy, float mutationRate);
Population crossover(const Guy &guy1, const Guy &guy2, float crossoverRate);
Solution genetic_solve(const Data &data, int populationSize, int nbGenerations, float mutationRate, float crossoverRate, int k);

#endif
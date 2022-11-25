#ifndef GENETIC_SEARCH_HPP
#define GENETIC_SEARCH_HPP

#include <vector>
#include "Graph/Graph.hpp"
#include "utils.hpp"

using Solution = std::vector<FighterVertex>;
using Fellow = std::vector<int>;
using Population = std::vector<Fellow>;
using Data = Graph;

Fellow generate_random_fellow(int nbChromosomes);
Solution decode_fellow(const Fellow &guy);
Fellow decode_solution(const Solution &solution, const Data &data);
int fitness(const Fellow &guy, const Data &data);
Fellow selection(const Population &population, std::vector<int> &scores, int k);
void mutation(Fellow guy, float mutationRate);
Population crossover(const Fellow &guy1, const Fellow &guy2, float crossoverRate);
Solution genetic_solve(const Data &data, int populationSize, int nbGenerations, float mutationRate, float crossoverRate, int k);

#endif
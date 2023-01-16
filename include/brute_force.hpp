#ifndef BRUTE_FORCE_HPP
#define BRUTE_FORCE_HPP

#include <vector>

#include "Graph/Graph.hpp"

int compute_lower_bound(const std::vector<FighterVertex> &fighterList, const std::vector<FireVertex> &fireList);
std::vector<FighterVertex> bruteforce_solve(const Graph &graph);
std::vector<FighterVertex> better_bruteforce_solve(const Graph &graph);
void generate_partitions(int n, int k, int i, int nb_selected, std::vector<bool>& partition, std::vector<std::vector<bool>>& partitions);

#endif
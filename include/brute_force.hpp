#ifndef BRUTE_FORCE_HPP
#define BRUTE_FORCE_HPP

#include <vector>
#include "Graph/Graph.hpp"

bool check_feasibility(const std::vector<FighterVertex> &fighterList, const std::vector<FireVertex> &fireList);
int compute_lower_bound(const std::vector<FighterVertex> &fighterList, const std::vector<FireVertex> &fireList);
std::vector<FighterVertex> bruteforce_solve(const Graph &graph);
std::vector<FighterVertex> bruteforce_solve_concon(const Graph &graph);
std::vector<std::vector<bool>> generate_partitions(int n, int k);

#endif
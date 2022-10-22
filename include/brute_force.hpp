#ifndef BRUTE_FORCE_HPP
#define BRUTE_FORCE_HPP

#include <vector>
#include "Graph/Graph.hpp"

bool check_feasibility(const std::vector<FighterVertex> &fighterList, const std::vector<FireVertex> &fireList);
int compute_lower_bound(const std::vector<FighterVertex> &fighterList, const std::vector<FireVertex> &fireList);
std::vector<FighterVertex> bruteforce_solve(const Graph &graph);

#endif
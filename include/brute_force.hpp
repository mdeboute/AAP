#ifndef BRUTE_FORCE_HPP
#define BRUTE_FORCE_HPP

#include <vector>
#include "Graph/Graph.hpp"

std::vector<std::vector<FighterVertex>> find_partitions(std::vector<FighterVertex> fighterList, std::vector<FireVertex> fireList);
bool check_feasibility(std::vector<FighterVertex> fighterList, std::vector<FireVertex> fireList);
int compute_lower_bound(std::vector<FighterVertex> fighterList, std::vector<FireVertex> fireList);

std::vector<FighterVertex> BFsolve(Graph& graph);

#endif
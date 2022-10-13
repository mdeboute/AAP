#ifndef BRUTE_FORCE_HPP
#define BRUTE_FORCE_HPP

#include <vector>
#include "Graph/FighterVertex.hpp"
#include "Graph/FireVertex.hpp"

std::vector<std::vector<FighterVertex>> findPartitions(std::vector<FighterVertex> fighterList);
bool checker(std::vector<FighterVertex> fighterList, std::vector<FireVertex> fireList);
std::vector<FighterVertex> solve(std::vector<std::vector<FighterVertex>> partitions, std::vector<FireVertex> fireList);

#endif
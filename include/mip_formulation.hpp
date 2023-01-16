#ifndef MIP_FORMULATION_HPP
#define MIP_FORMULATION_HPP

#include "utils.hpp"
#include "gurobi_c++.h"

#include <cstring>
#include <cmath>
#include <vector>

const std::vector<FighterVertex> mip_solve(
    Graph &graph, bool verbose, int time_limit);

#endif
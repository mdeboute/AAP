#ifndef MIP_FORMULATION_HPP
#define MIP_FORMULATION_HPP

#include "gurobi_c++.h"
#include <cstring>
#include <cmath>
#include <vector>
#include "utils.hpp"

const std::vector<FighterVertex> mip_solve(
    Graph &graph, bool verbose);

#endif
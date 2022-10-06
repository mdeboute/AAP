#ifndef MIP_FORMULATION_HPP
#define MIP_FORMULATION_HPP

#include "gurobi_c++.h"
#include "file_io.hpp"
#include <cstring>
#include <cmath>
#include <vector>

std::vector<std::vector<Color>> solve(std::vector<std::vector<Color>> map, std::vector<float> config);

#endif
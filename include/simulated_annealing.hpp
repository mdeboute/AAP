#ifndef SIMULATED_ANNEALING_HPP
#define SIMULATED_ANNEALING_HPP

#include <vector>

#include "Graph/Graph.hpp"
#include "utils.hpp"

using Team = std::vector<FighterVertex>;
using Solution = std::vector<int>;
using Neighborhood = std::vector<Solution>;
using Data = Graph;

Team decode_solution(const Solution& solution, const Data& data);
Solution decode_team(const Team &team, const Data &data);
int get_cost(const Solution &solution, const Data &data);
Solution pick_and_drop(const Solution &solution);
Team remove_useless_fire_fighters(const Team &team, const Data &data);
Team sa_solve(const Data &data, int max_iter, int initial_temperature, int final_temparature, float cooling_rate);
int get_initial_temperature(Data &data, float tau, int max_iter, int neighborhood_size);
#endif
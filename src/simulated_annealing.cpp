#include "simulated_annealing.hpp"
#include "greedy.hpp"

#include <climits>
#include <numeric>
#include <chrono>

Team decode_solution(const Solution &solution, const Data &data)
{
    Team team;
    for (int i = 0; i < solution.size(); i++)
    {
        if (solution[i] == 1)
        {
            team.push_back(data.getFigtherVertexList()[i]);
        }
    }
    return team;
}

Solution decode_team(const Team &team, const Data &data)
{
    Solution solution;
    for (int i = 0; i < data.getFigtherVertexList().size(); i++)
    {
        solution.push_back(0);
    }
    for (int i = 0; i < team.size(); i++)
    {
        for (int j = 0; j < data.getFigtherVertexList().size(); j++)
        {
            if (team[i] == data.getFigtherVertexList()[j])
            {
                solution[j] = 1;
            }
        }
    }
    return solution;
}

int get_cost(const Solution &solution, const Data &data)
{
    Team team = decode_solution(solution, data);
    if (check_feasibility(team, data.getFireVertexList()))
    {
        return team.size();
    }
    return INT_MAX;
}

Solution pick_and_drop(const Solution &solution)
{
    // pick a random index and change its value (0 -> 1 or 1 -> 0)
    Solution new_solution = solution;
    int index = rand() % solution.size();
    new_solution[index] = 1 - new_solution[index];
    return new_solution;
}

Team remove_useless_fire_fighters(const Team &team, const Data &data)
{
    // try to remove useless fire fighters by decreasing the number of fire fighters
    // by selecting the fighter who covers the least number of fires
    // and checking if the solution is still feasible

    // sort the list by the fighter who covers the least number of fires
    Team sorted_team = team;
    std::sort(sorted_team.begin(), sorted_team.end(), [](const FighterVertex &f1, const FighterVertex &f2)
              { return f1.getNbFireCovered() < f2.getNbFireCovered(); });

    // try to remove the maximum number of fire fighters and check if the solution is still feasible
    // do it recursively
    Team new_team = sorted_team;
    for (int i = 0; i < sorted_team.size(); i++)
    {
        new_team.erase(new_team.begin() + i);
        if (check_feasibility(new_team, data.getFireVertexList()))
        {
            return remove_useless_fire_fighters(new_team, data);
        }
        new_team = sorted_team;
    }
    return sorted_team;
}

Team sa_solve(const Data &data, int max_iter, int initial_temperature, int final_temparature, float cooling_rate)
{
    auto startingTime = std::chrono::steady_clock::now();
    Team best_team = greedy_solve(data, false);
    Solution best_sol = decode_team(best_team, data);
    int best_cost = get_cost(best_sol, data);
    int current_temperature = initial_temperature;
    int iter = 0;
    while (current_temperature > final_temparature)
    {
        Solution current_sol = best_sol;
        while (iter < max_iter)
        {
            Solution sol = pick_and_drop(current_sol);
            int cost = get_cost(sol, data);
            int delta = cost - best_cost;
            if ((delta < 0) || (rand() % 1 <= exp(-delta / current_temperature)))
            {
                best_sol = sol;
                best_cost = cost;
            }
            iter++;
        }
        current_temperature *= cooling_rate;
        iter = 0;
    }
    best_team = decode_solution(best_sol, data);
    best_team = remove_useless_fire_fighters(best_team, data);
    std::chrono::duration<double> tt = std::chrono::steady_clock::now() - startingTime;
    std::cout << "Result: runtime = " << tt.count() << " sec; objective value = " << best_team.size() << std::endl;
    std::cout << std::endl;
    return best_team;
}

int get_initial_temperature(Data &data, float tau, int max_iter, int neighborhood_size)
{
    Team team = greedy_solve(data, false);
    Solution solution = decode_team(team, data);
    Neighborhood neighborhood;
    for (int i = 0; i < neighborhood_size; i++)
    {
        neighborhood.push_back(pick_and_drop(solution));
    }
    std::vector<int> deltas;
    for (int i = 0; i < neighborhood.size(); i++)
    {
        Solution sol_1 = neighborhood[rand() % neighborhood.size()];
        Solution sol_2 = neighborhood[rand() % neighborhood.size()];
        int cost_1 = get_cost(sol_1, data);
        int cost_2 = get_cost(sol_2, data);
        deltas.push_back(abs(cost_1 - cost_2));
    }
    int mean_delta = std::accumulate(deltas.begin(), deltas.end(), 0) / deltas.size();
    return -mean_delta / log(tau);
}

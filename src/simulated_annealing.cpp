#include "simulated_annealing.hpp"
#include "greedy.hpp"
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
    return __INT_MAX__;
}

Solution single_swap(const Solution &solution)
{
    // take a two random indices and swap their values
    Solution new_solution = solution;
    int index_1 = rand() % solution.size();
    int index_2 = rand() % solution.size();
    int temp = new_solution[index_1];
    new_solution[index_1] = new_solution[index_2];
    new_solution[index_2] = temp;
    return new_solution;
}

Solution pick_and_drop(const Solution &solution)
{
    // pick a random index and change its value (0 -> 1 or 1 -> 0)
    Solution new_solution = solution;
    int index = rand() % solution.size();
    new_solution[index] = 1 - new_solution[index];
    return new_solution;
}

Solution random_movement(const Solution &solution)
{
    if (rand() % 2 == 0)
    {
        return single_swap(solution);
    }
    return pick_and_drop(solution);
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
            Solution sol = random_movement(current_sol);
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
    std::chrono::duration<double> tt = std::chrono::steady_clock::now() - startingTime;
    std::cout << "Result: runtime = " << tt.count() << " sec; objective value = " << best_team.size() << std::endl;
    std::cout << std::endl;
    return best_team;
}
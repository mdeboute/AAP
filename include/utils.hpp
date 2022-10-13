#ifndef UTILS_HPP
#define UTILS_HPP

#include <cmath>
#include <vector>
#include "file_io.hpp"
#include "Graph/FighterVertex.hpp"
#include "Graph/FireVertex.hpp"
#include "Graph/Position.hpp"
#include "Graph/Graph.hpp"

enum direction
{
    LEFT = -1,
    RIGHT = 1,
};

struct pixel
{
    int x;
    int y;
};

struct ray
{
    pixel source;
    float slope;
    float intercept;
    direction dir;
    pixel target;
};

std::vector<pixel> circle_to_pixels(pixel center, float radius, int width, int height);
std::vector<pixel> calculate_ray_path(std::vector<std::vector<Color>> map, ray ray);
std::vector<pixel> calculate_ray_neighborhood(std::vector<std::vector<int>> feasibility_map, std::vector<pixel> ray_path, float action_radius, int ray_index, std::vector<std::vector<std::vector<int>>> &ray_fighting_map);
void display_map(std::vector<std::vector<Color>> map);
std::vector<FighterVertex> cutUselessFighters(std::vector<FighterVertex> fighterList);
std::vector<std::string> splitString(const std::string &s, const std::string &delim);
Graph calculate_graph_data(std::vector<std::vector<Color>> map, std::vector<float> config);

#endif
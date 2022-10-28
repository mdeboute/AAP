#ifndef UTILS_HPP
#define UTILS_HPP

#define _USE_MATH_DEFINES
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


std::vector<Position> circle_to_positions(Position center, float radius, int width, int height);
std::vector<Position> calculate_ray_path(const std::vector<std::vector<Color>> &map, Position source, float slope, float intercept, direction dir);

void calculate_ray_neighborhood(
    const std::vector<std::vector<int>> &feasibilityMap,
    const std::vector<Position> &rayPath,
    float actionRadius,
    int rayIndex,
    std::vector<std::vector<std::vector<int>>> &rayFightingMap);

void display_map(const std::vector<std::vector<Color>> &map);

std::vector<std::string> split_string(const std::string &s, const std::string &delim);

Graph calculate_graph_data(
    std::vector<std::vector<Color>> &map,
    const std::vector<float> &config, bool isReduced, bool addAdjacency);

const std::vector<std::vector<Color>> &draw_details(
    std::vector<std::vector<Color>> &map,
    const std::vector<float> &config);

#endif
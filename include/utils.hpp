#ifndef UTILS_HPP
#define UTILS_HPP

#include <cmath>
#include <vector>
#include "file_io.hpp"

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

#endif
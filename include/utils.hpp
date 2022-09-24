#ifndef UTILS_HPP
#define UTILS_HPP

#include<cmath>
#include<vector>

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
std::vector<pixel> calculate_ray_path(std::vector<std::vector<int>> map, ray ray);
std::vector<pixel> calculate_ray_neighborhood(std::vector<std::vector<int>> feasibility_map, ray ray, std::vector<pixel> ray_path);

#endif
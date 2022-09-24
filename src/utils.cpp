#include "utils.hpp"


std::vector<pixel> circle_to_pixels(pixel center, float radius, int width, int height)
{
    // possibly switch to float center coordinates and cast (int) to get pixels
    int x_bound_a = (int)floor(center.x + 0.5 - radius);
    int x_bound_b = (int)ceil(center.x + 0.5 + radius);
    int y_bound_a = (int)floor(center.y + 0.5 - radius);
    int y_bound_b = (int)ceil(center.y + 0.5 + radius);

    std::vector<pixel> pixels;

    for (int y = y_bound_a; y < y_bound_b; y++)
    {
        for (int x = x_bound_a; x < x_bound_b; x++)
        {
            if (x == center.x || y == center.y ||
                (x < center.x && y > center.y && // North-West
                 pow(x + 1 - (center.x + 0.5), 2) + pow(y - (center.y + 0.5), 2) <= pow(radius, 2)) ||
                (x < center.x && y < center.y && // South-West
                 pow(x + 1 - (center.x + 0.5), 2) + pow(y + 1 - (center.y + 0.5), 2) <= pow(radius, 2)) ||
                (x > center.x && y < center.y && // South-East
                 pow(x - (center.x + 0.5), 2) + pow(y + 1 - (center.y + 0.5), 2) <= pow(radius, 2)) ||
                (x > center.x && y > center.y && // North-East
                 pow(x - (center.x + 0.5), 2) + pow(y - (center.y + 0.5), 2) <= pow(radius, 2)))
            {
                if (x >= 0 && x < width && y >= 0 && y < height)
                {
                    pixel pixel;
                    pixel.x = x;
                    pixel.y = y;
                    pixels.push_back(pixel);
                }
            }
        }
    }

    return pixels;
}

std::vector<pixel> calculate_ray_path(std::vector<std::vector<int>> map, ray ray)
{
    std::vector<pixel> path;
    bool obstacle_reached = false;
    int x = ray.source.x;
    int curr_y = (int)floor(ray.slope * x + ray.intercept);

    while (!obstacle_reached)
    {
        int next_x = x + ray.dir;
        int next_y = (int)ceil(ray.slope * next_x + ray.intercept);

        int step = (int)(next_y - curr_y) / abs(next_y - curr_y);

        for (int y = curr_y; y != next_y; y += step)
        {
            pixel pixel;
            pixel.x = x;
            pixel.y = y;
            path.push_back(pixel);

            if (map[y][x] == BLUE || map[y][x] == BLACK || y + step < 0 || y + step >= map.size())
            {
                obstacle_reached = true;
                break;
            }
        }
        x = next_x;
        curr_y = next_y;
    }

    return path;
}

std::vector<pixel> calculate_ray_neighborhood(std::vector<std::vector<int>> feasibility_map, ray ray, std::vector<pixel> ray_path)
{
    std::vector<pixel> neighborhood;

    // get feasible placements for fire fighters to stop ray ray

    return neighborhood;
}

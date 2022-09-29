#include "utils.hpp"
#include "parser.hpp"

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
    // std::cout << "Ray(" << ray.source.x << "," << ray.source.y << ")" << ray.dir << ": y = " << ray.slope << "*x + " << ray.intercept << " in path vector" << std::endl;  //display current ray degrees

    std::vector<pixel> path;
    bool obstacle_reached = false;
    int x = ray.source.x;
    int curr_y = ray.source.y;
    // Check these to make sure all values are accounted for
    while (!obstacle_reached)
    {
        int next_x = x + ray.dir;
        int next_y;
        
        if (ray.slope == INFINITY)
            next_y = map.size();
        else if (ray.slope == -INFINITY)
            next_y = 0;
        else
            next_y = (int)ceil(ray.slope * next_x + ray.intercept);

        if (next_y == curr_y)
            next_y = curr_y + 1;

        int step = (int)(next_y - curr_y) / abs(next_y - curr_y); // +1 or -1 depending on the direction of the ray

        
        for (int y = curr_y; y != next_y; y += step)
        {
            //std::cout << " y is " << y << " next y is " << y + step << " >/< " << next_y << std::endl;
            pixel pixel;
            pixel.x = x;
            pixel.y = y;
            //std::cout << "Trying to put pixel " << x << "," << y << " in path vector : ";
            path.push_back(pixel);
            //std::cout << "Success!" << std::endl;
            if (map[y][x] == BLUE || map[y][x] == BLACK || y + step < 0 || y + step >= map.size())
            {
                obstacle_reached = true;
                break;
            }
        }

        x = next_x;
        if (x < 0 || x >= map[0].size())
            break;
        curr_y = (int)floor(ray.slope * x + ray.intercept);
        if (curr_y < 0 || curr_y >= map.size())
            break;
    }

    return path;
}

std::vector<pixel> calculate_ray_neighborhood(std::vector<std::vector<int>> feasibility_map, std::vector<pixel> ray_path, float action_radius)
{
    // make sure feasibility_map is a copy
    std::vector<pixel> neighborhood;
    int height = feasibility_map.size();
    int width = feasibility_map[0].size();

    for (int p = 0; p < ray_path.size(); p++)
    {
        std::vector<pixel> circle_pixels = circle_to_pixels(ray_path[p], action_radius, width, height);
        for (const pixel &pixel : circle_pixels)
        {
            if (feasibility_map[pixel.y][pixel.x] == 1)
            {
                neighborhood.push_back(pixel);
                feasibility_map[pixel.y][pixel.x] = 0;
            }
        }
    }

    return neighborhood;
}

void display_map(std::vector<std::vector<int>> map)
{
    for (int y = 0; y < map.size(); y++)
    {
        for (int x = 0; x < map[y].size(); x++)
        {
            if (map[y][x] == BLUE)
                std::cout << "~";
            if (map[y][x] == YELLOW)
                std::cout << " ";
            if (map[y][x] == BLACK)
                std::cout << "$";
            if (map[y][x] == RED)
                std::cout << "X";
            if (map[y][x] == GREEN)
                std::cout << "O";
            if (map[y][x] == ORANGE)
                std::cout << "*";
            if (map[y][x] == LIME)
                std::cout << "0";
            if (map[y][x] == MAGENTA)
                std::cout << "%";
        }
        std::cout << std::endl;
    }
}

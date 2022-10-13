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

std::vector<pixel> calculate_ray_path(std::vector<std::vector<Color>> map, ray ray)
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
        float exact_next_y;

        if (ray.slope == INFINITY)
            exact_next_y = map.size();
        else if (ray.slope == -INFINITY)
            exact_next_y = 0;
        else
            exact_next_y = ray.slope * next_x + ray.intercept;
        
        if ((ray.slope >=0 && ray.dir==RIGHT) || (ray.slope <0 && ray.dir==LEFT))
            next_y = (int) ceil(exact_next_y);
        else
            next_y = (int) floor(exact_next_y);

        if (next_y == curr_y)
            next_y = curr_y + ray.dir;

        int step = (int)(next_y - curr_y) / abs(next_y - curr_y); // +1 or -1 depending on the direction of the ray

        for (int y = curr_y; y != next_y; y += step)
        {
            // std::cout << " y is " << y << " next y is " << y + step << " >/< " << next_y << std::endl;
            pixel pixel;
            pixel.x = x;
            pixel.y = y;
            // std::cout << "Trying to put pixel " << x << "," << y << " in path vector : ";
            path.push_back(pixel);
            // std::cout << "Success!" << std::endl;
            if (map[y][x] == BLUE || map[y][x] == BLACK || y + step < 0 || y + step >= map.size())
            {
                obstacle_reached = true;
                break;
            }
        }

        x = next_x;
        if (x < 0 || x >= map[0].size())
            break;
        
        if ((ray.slope >=0 && ray.dir==RIGHT) || (ray.slope <0 && ray.dir==LEFT))
            curr_y = (int) floor(exact_next_y);
        else
            curr_y = (int) ceil(exact_next_y);
        if (curr_y < 0 || curr_y >= map.size())
            break;
    }

    return path;
}

std::vector<pixel> calculate_ray_neighborhood(std::vector<std::vector<int>> feasibility_map, std::vector<pixel> ray_path, float action_radius, int ray_index, std::vector<std::vector<std::vector<int>>> &ray_fighting_map)
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
                ray_fighting_map[pixel.y][pixel.x].push_back(ray_index);
            }
        }
    }

    return neighborhood;
}

void display_map(std::vector<std::vector<Color>> map)
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

std::vector<std::string> splitString(const std::string &s, const std::string &delim)
{
    std::vector<std::string> elems;
    size_t last = 0;
    size_t next = 0;
    while ((next = s.find(delim, last)) != std::string::npos)
    {
        elems.push_back(s.substr(last, next - last));
        last = next + 1;
    }
    elems.push_back(s.substr(last));
    return elems;
}

Graph calculate_graph_data(std::vector<std::vector<Color>> map, std::vector<float> config)
{
    int nb_rays = (int)config[0];
    // int nb_rays = 25;
    float furnace_radius = config[1];
    float action_radius = config[2];

    size_t height = map.size();
    size_t width = map[0].size();

    std::vector<pixel> fire_centers;
    std::vector<std::vector<int>> feasibility_map;
    std::vector<std::vector<std::vector<int>>> ray_fighting_map;
    std::cout << "Start gathering data" << std::endl;
    // We get the fire centers and a map overlay of places we can't put firefighters
    for (size_t y = 0; y < height; y++)
    {
        std::vector<int> feasibility_map_line;
        std::vector<std::vector<int>> ray_fighting_map_line;
        for (size_t x = 0; x < width; x++)
        {
            if (map[y][x] == YELLOW)
                feasibility_map_line.push_back(1);
            else
                feasibility_map_line.push_back(0); // to changer if firefighters can be in cities
            if (map[y][x] == RED)                  //
            {
                pixel fire;
                fire.x = x;
                fire.y = y;
                fire_centers.push_back(fire);
            }
            std::vector<int> ray_fighting_pos;
            ray_fighting_map_line.push_back(ray_fighting_pos);
        }
        feasibility_map.push_back(feasibility_map_line);
        ray_fighting_map.push_back(ray_fighting_map_line);
    }

    size_t nb_fires = fire_centers.size();

    std::cout << "Finished gathering firecenters and partial feasible placement data" << std::endl;

    for (size_t f = 0; f < nb_fires; f++)
    {
        std::vector<pixel> furnace = circle_to_pixels(fire_centers[f], furnace_radius, width, height);
        for (auto &&pixel : furnace)
        {
            feasibility_map[pixel.y][pixel.x] = 0;
            if (map[pixel.y][pixel.x] != BLUE && map[pixel.y][pixel.x] != RED)
                map[pixel.y][pixel.x] = MAGENTA;
        }
    }

    std::cout << "Finished gathering fire furnace areas and removed them from feasible placements" << std::endl;

    std::vector<std::vector<ray>> fire_rays;
    std::vector<std::vector<std::vector<pixel>>> fire_ray_paths;
    std::vector<std::vector<pixel>> fatal_ray_neighborhoods;
    std::vector<ray> fatal_rays;

    for (size_t f = 0; f < nb_fires; f++)
    {
        std::vector<ray> rays;
        std::vector<std::vector<pixel>> ray_paths;
        for (size_t r = 0; r < nb_rays; r++)
        {
            float degrees = r * 360.0 / nb_rays;
            // std::cout << degrees;  //display current ray degrees
            float x_r = fire_centers[f].x + 0.5 + furnace_radius * cos(degrees * (M_PI / 180.0));
            float y_r = fire_centers[f].y + 0.5 + furnace_radius * sin(degrees * (M_PI / 180.0));
            // std::cout << " 2(" << x_r << ", " << y_r << ")" << std::endl;
            ray ray;
            ray.slope = (y_r - (fire_centers[f].y + 0.5)) / (x_r - (fire_centers[f].x + 0.5));
            ray.intercept = y_r - ray.slope * x_r;
            ray.source = fire_centers[f]; // possible copy of data. Can be improved later
            if (degrees > 90 && degrees <= 270)
                ray.dir = LEFT;
            else
                ray.dir = RIGHT;

            // std::cout << "Ray " << r << ": y = " << ray.slope << " * x + " << ray.intercept << " from(" << ray.source.x << ", " << ray.source.y << ")";

            std::vector<pixel> ray_path = calculate_ray_path(map, ray);

            // for (size_t i = 1; i < ray_path.size()-1; i++)
            //     if(map[ray_path[i].y][ray_path[i].x] == YELLOW)
            //         map[ray_path[i].y][ray_path[i].x] = ORANGE;

            ray.target = ray_path[ray_path.size() - 1];

            // std::cout << " to(" << ray.target.x << ", " << ray.target.y << ") in direction " << ray.dir << std::endl;  //display current ray

            if (map[ray.target.y][ray.target.x] == BLACK) // ray is directed to a city
            {
                std::vector<pixel> ray_neighborhood = calculate_ray_neighborhood(feasibility_map, ray_path, action_radius, (int)fatal_ray_neighborhoods.size(), ray_fighting_map);
                fatal_ray_neighborhoods.push_back(ray_neighborhood);
                fatal_rays.push_back(ray);
            }
            rays.push_back(ray);
            ray_paths.push_back(ray_path);
        }
        fire_rays.push_back(rays);
        fire_ray_paths.push_back(ray_paths);
    }

    std::vector<FireVertex> fireTab;
    std::vector<FighterVertex> fighterTab;

    for (int r=0; r<fatal_rays.size(); r++){
        ray ray = fatal_rays[r];
        Position fireCenter(ray.source.x, ray.source.y);
        Position collide(ray.target.x, ray.target.y);
        fireTab.push_back(FireVertex(fireCenter, collide, r));
    }
    int fighterID = 0;
    for (int y = 0; y < height; y++){
        for (int x=0; x < width; x++){
            if (ray_fighting_map[y][x].size() > 0){
                Position p(x, y);
                FighterVertex fighter(p, fighterID);
                for(int r : ray_fighting_map[y][x]){
                    fighter.addFire(fireTab[r]);
                }
                fighterTab.push_back(fighter);
                fighterID++;
            }
        }
    }
    return Graph(fireTab, fighterTab);
}
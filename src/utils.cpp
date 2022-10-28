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

std::vector<pixel> calculate_ray_path(const std::vector<std::vector<Color>> &map, ray ray)
{
    std::vector<pixel> path;
    bool obstacleReached = false;
    int x = ray.source.x;
    int curr_y = ray.source.y;

    while (!obstacleReached)
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

        if ((ray.slope >= 0 && ray.dir == RIGHT) || (ray.slope < 0 && ray.dir == LEFT))
            next_y = (int)ceil(exact_next_y);
        else
            next_y = (int)floor(exact_next_y);

        if (next_y == curr_y)
            next_y = curr_y + ray.dir;

        int step = (int)(next_y - curr_y) / abs(next_y - curr_y); // +1 or -1 depending on the direction of the ray

        for (int y = curr_y; y != next_y; y += step)
        {
            pixel pixel;
            pixel.x = x;
            pixel.y = y;
            path.push_back(pixel);

            if (map[y][x] == BLUE || map[y][x] == BLACK || y + step < 0 || y + step >= map.size())
            {
                obstacleReached = true;
                break;
            }
        }

        x = next_x;
        if (x < 0 || x >= map[0].size())
            break;

        if ((ray.slope >= 0 && ray.dir == RIGHT) || (ray.slope < 0 && ray.dir == LEFT))
            curr_y = (int)floor(exact_next_y);
        else
            curr_y = (int)ceil(exact_next_y);
        if (curr_y < 0 || curr_y >= map.size())
            break;
    }

    return path;
}

std::vector<pixel> calculate_ray_neighborhood(const std::vector<std::vector<int>> &feasibilityMap,
                                              const std::vector<pixel> &rayPath,
                                              float actionRadius,
                                              int rayIndex,
                                              std::vector<std::vector<std::vector<int>>> &rayFightingMap)
{
    // make sure feasibility_map is a copy
    std::vector<pixel> neighborhood;
    std::vector<std::vector<int>> explorationMap = feasibilityMap;
    int height = feasibilityMap.size();
    int width = feasibilityMap[0].size();

    for (int p = 0; p < rayPath.size() - 1; p++)
    {
        if (feasibilityMap[rayPath[p].y][rayPath[p].x] == 0)
            continue;

        std::vector<pixel> circle_pixels = circle_to_pixels(rayPath[p], actionRadius, width, height);
        for (const pixel &pixel : circle_pixels)
        {
            if (explorationMap[pixel.y][pixel.x] == 1)
            {
                neighborhood.push_back(pixel);
                explorationMap[pixel.y][pixel.x] = 0;
                rayFightingMap[pixel.y][pixel.x].push_back(rayIndex);
            }
        }
    }

    return neighborhood;
}

void display_map(const std::vector<std::vector<Color>> &map)
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

const std::vector<std::vector<Color>> &draw_details(std::vector<std::vector<Color>> &map, const std::vector<float> &config)
{
    int nbRays = (int)config[0];
    float furnaceRadius = config[1];
    float actionRadius = config[2];

    size_t height = map.size();
    size_t width = map[0].size();

    std::vector<pixel> fireCenters;
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            if (map[y][x] == RED) //
            {
                pixel fire;
                fire.x = x;
                fire.y = y;
                fireCenters.push_back(fire);
                std::vector<pixel> furnace = circle_to_pixels(fire, furnaceRadius, width, height);
                for (auto &&pixel : furnace)
                    if (map[pixel.y][pixel.x] != BLUE && map[pixel.y][pixel.x] != RED)
                        map[pixel.y][pixel.x] = MAGENTA;
            }
            if (map[y][x] == GREEN)
            {
                pixel firefighter;
                firefighter.x = x;
                firefighter.y = y;
                std::vector<pixel> action_area = circle_to_pixels(firefighter, actionRadius, width, height);
                for (auto &&p : action_area)
                    if (map[p.y][p.x] == YELLOW || map[p.y][p.x] == ORANGE)
                        map[p.y][p.x] = LIME;
                map[y][x] = GREEN;
            }
        }
    }

    size_t nbFires = fireCenters.size();

    for (size_t f = 0; f < nbFires; f++)
    {
        for (size_t r = 0; r < nbRays; r++)
        {
            float degrees = r * 360.0 / nbRays;
            float x_r = fireCenters[f].x + 0.5 + furnaceRadius * cos(degrees * (M_PI / 180.0));
            float y_r = fireCenters[f].y + 0.5 + furnaceRadius * sin(degrees * (M_PI / 180.0));
            ray ray;
            ray.slope = (y_r - (fireCenters[f].y + 0.5)) / (x_r - (fireCenters[f].x + 0.5));
            ray.intercept = y_r - ray.slope * x_r;
            ray.source = fireCenters[f]; // possible copy of data. Can be improved later
            if (degrees > 90 && degrees <= 270)
                ray.dir = LEFT;
            else
                ray.dir = RIGHT;

            std::vector<pixel> rayPath = calculate_ray_path(map, ray);

            ray.target = rayPath[rayPath.size() - 1];

            for (size_t i = 1; i < rayPath.size(); i++)
            {
                pixel p = rayPath[i];
                if (map[p.y][p.x] == LIME || map[p.y][p.x] == GREEN || map[p.y][p.x] == BLACK)
                    break;
                if (map[p.y][p.x] == YELLOW)
                    map[p.y][p.x] = ORANGE;
            }
        }
    }
    return map;
}

std::vector<std::string> split_string(const std::string &s, const std::string &delim)
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

Graph calculate_graph_data(std::vector<std::vector<Color>> &map, const std::vector<float> &config, bool isReduced, bool addAdjacency)
{
    int nbRays = (int)config[0];
    float furnaceRadius = config[1];
    float actionRadius = config[2];

    size_t height = map.size();
    size_t width = map[0].size();

    std::vector<pixel> fireCenters;
    std::vector<std::vector<int>> feasibilityMap;
    std::vector<std::vector<std::vector<int>>> rayFightingMap;
    std::cout << "Start gathering data..." << std::endl;

    // We get the fire centers and a map overlay of places we can't put firefighters
    for (size_t y = 0; y < height; y++)
    {
        std::vector<int> feasibilityMapLine;
        std::vector<std::vector<int>> rayFightingMapLine;
        for (size_t x = 0; x < width; x++)
        {
            if (map[y][x] == YELLOW)
                feasibilityMapLine.push_back(1);
            else
                feasibilityMapLine.push_back(0); // to change if firefighters can be in cities
            if (map[y][x] == RED)
            {
                pixel fire;
                fire.x = x;
                fire.y = y;
                fireCenters.push_back(fire);
            }
            std::vector<int> ray_fighting_pos;
            rayFightingMapLine.push_back(ray_fighting_pos);
        }
        feasibilityMap.push_back(feasibilityMapLine);
        rayFightingMap.push_back(rayFightingMapLine);
    }

    size_t nbFires = fireCenters.size();

    std::cout << "Finished gathering firecenters and partial feasible placement data!" << std::endl;

    for (size_t f = 0; f < nbFires; f++)
    {
        std::vector<pixel> furnace = circle_to_pixels(fireCenters[f], furnaceRadius, width, height);
        for (auto &&pixel : furnace)
        {
            feasibilityMap[pixel.y][pixel.x] = 0;
            if (map[pixel.y][pixel.x] != BLUE && map[pixel.y][pixel.x] != RED)
                map[pixel.y][pixel.x] = MAGENTA;
        }
    }

    std::cout << "Finished gathering fire furnace areas and removed them from feasible placements!\n"
              << std::endl;

    std::vector<std::vector<ray>> fireRays;
    std::vector<std::vector<std::vector<pixel>>> fireRayPaths;
    std::vector<std::vector<pixel>> fatalRayNeighborhoods;
    std::vector<ray> fatalRays;

    for (size_t f = 0; f < nbFires; f++)
    {
        std::vector<ray> rays;
        std::vector<std::vector<pixel>> rayPaths;
        for (size_t r = 0; r < nbRays; r++)
        {
            float degrees = r * 360.0 / nbRays;
            float x_r = fireCenters[f].x + 0.5 + furnaceRadius * cos(degrees * (M_PI / 180.0));
            float y_r = fireCenters[f].y + 0.5 + furnaceRadius * sin(degrees * (M_PI / 180.0));
            ray ray;
            ray.slope = (y_r - (fireCenters[f].y + 0.5)) / (x_r - (fireCenters[f].x + 0.5));
            ray.intercept = y_r - ray.slope * x_r;
            ray.source = fireCenters[f]; // possible copy of data. Can be improved later
            if (degrees > 90 && degrees <= 270)
                ray.dir = LEFT;
            else
                ray.dir = RIGHT;

            std::vector<pixel> rayPath = calculate_ray_path(map, ray);

            ray.target = rayPath[rayPath.size() - 1];

            if (map[ray.target.y][ray.target.x] == BLACK) // ray is directed to a city
            {
                std::vector<pixel> rayNeighborhood = calculate_ray_neighborhood(feasibilityMap, rayPath, actionRadius, (int)fatalRayNeighborhoods.size(), rayFightingMap);
                fatalRayNeighborhoods.push_back(rayNeighborhood);
                fatalRays.push_back(ray);
            }
            rays.push_back(ray);
            rayPaths.push_back(rayPath);
        }
        fireRays.push_back(rays);
        fireRayPaths.push_back(rayPaths);
    }

    std::vector<FireVertex> fireList;
    std::vector<FighterVertex> fighterList;

    for (int r = 0; r < fatalRays.size(); r++)
    {
        ray ray = fatalRays[r];
        Position fireCenter(ray.source.x, ray.source.y);
        Position collide(ray.target.x, ray.target.y);
        fireList.push_back(FireVertex(fireCenter, collide, r, r));
    }
    int fighterID = 0;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (rayFightingMap[y][x].size() > 0)
            {
                Position p(x, y);
                FighterVertex fighter(p, fighterID, fighterID);
                for (int r : rayFightingMap[y][x])
                {
                    fighter.addFire(fireList[r]);
                }
                fighterList.push_back(fighter);
                fighterID++;
            }
        }
    }
    return Graph(fireList, fighterList, isReduced, addAdjacency);
}
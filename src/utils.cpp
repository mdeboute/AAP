#include "utils.hpp"

std::vector<Position> circle_to_positions(Position center, float radius, int width, int height)
{
    // possibly switch to float center coordinates and cast (int) to get pixels
    int x_bound_a = (int)floor(center.getX() + 0.5 - radius);
    int x_bound_b = (int)ceil(center.getX() + 0.5 + radius);
    int y_bound_a = (int)floor(center.getY() + 0.5 - radius);
    int y_bound_b = (int)ceil(center.getY() + 0.5 + radius);

    std::vector<Position> pixels;

    for (int y = y_bound_a; y < y_bound_b; y++)
    {
        for (int x = x_bound_a; x < x_bound_b; x++)
        {
            if (x == center.getX() || y == center.getY() ||
                (x < center.getX() && y > center.getY() && // North-West
                 pow(x + 1 - (center.getX() + 0.5), 2) + pow(y - (center.getY() + 0.5), 2) <= pow(radius, 2)) ||
                (x < center.getX() && y < center.getY() && // South-West
                 pow(x + 1 - (center.getX() + 0.5), 2) + pow(y + 1 - (center.getY() + 0.5), 2) <= pow(radius, 2)) ||
                (x > center.getX() && y < center.getY() && // South-East
                 pow(x - (center.getX() + 0.5), 2) + pow(y + 1 - (center.getY() + 0.5), 2) <= pow(radius, 2)) ||
                (x > center.getX() && y > center.getY() && // North-East
                 pow(x - (center.getX() + 0.5), 2) + pow(y - (center.getY() + 0.5), 2) <= pow(radius, 2)))
            {
                if (x >= 0 && x < width && y >= 0 && y < height)
                {
                    Position pixel(x, y);
                    pixels.push_back(pixel);
                }
            }
        }
    }

    return pixels;
}

std::vector<Position> calculate_ray_path(const std::vector<std::vector<Color>> &map, Position source, float slope, float intercept, direction dir)
{
    std::vector<Position> path;
    bool obstacleReached = false;
    int x = source.getX();
    int curr_y = source.getY();

    while (!obstacleReached)
    {
        int next_x = x + dir;
        int next_y;
        float exact_next_y;

        if (slope == INFINITY)
            exact_next_y = map.size();
        else if (slope == -INFINITY)
            exact_next_y = 0;
        else
            exact_next_y = slope * next_x + intercept;

        if ((slope >= 0 && dir == RIGHT) || (slope < 0 && dir == LEFT))
            next_y = (int)ceil(exact_next_y);
        else
            next_y = (int)floor(exact_next_y);

        if (next_y == curr_y)
            next_y = curr_y + dir;

        int step = (int)(next_y - curr_y) / abs(next_y - curr_y); // +1 or -1 depending on the direction of the ray

        for (int y = curr_y; y != next_y; y += step)
        {
            Position pixel(x, y);
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

        if ((slope >= 0 && dir == RIGHT) || (slope < 0 && dir == LEFT))
            curr_y = (int)floor(exact_next_y);
        else
            curr_y = (int)ceil(exact_next_y);
        if (curr_y < 0 || curr_y >= map.size())
            break;
    }

    return path;
}

void calculate_ray_neighborhood(const std::vector<std::vector<int>> &feasibilityMap,
                                const std::vector<Position> &rayPath,
                                float actionRadius,
                                int rayIndex,
                                std::vector<std::vector<std::vector<int>>> &rayFightingMap)
{
    // make sure feasibility_map is a copy
    std::vector<std::vector<int>> explorationMap = feasibilityMap;
    int height = feasibilityMap.size();
    int width = feasibilityMap[0].size();

    for (int p = 0; p < rayPath.size() - 1; p++)
    {
        if (feasibilityMap[rayPath[p].getY()][rayPath[p].getX()] == 0)
            continue;

        std::vector<Position> circle_positions = circle_to_positions(rayPath[p], actionRadius, width, height);
        for (const Position &pixel : circle_positions)
        {
            if (explorationMap[pixel.getY()][pixel.getX()] == 1)
            {
                explorationMap[pixel.getY()][pixel.getX()] = 0;
                rayFightingMap[pixel.getY()][pixel.getX()].push_back(rayIndex);
            }
        }
    }
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

    std::vector<Position> fireCenters;
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            if (map[y][x] == RED) //
            {
                Position fire(x, y);
                fireCenters.push_back(fire);
                std::vector<Position> furnace = circle_to_positions(fire, furnaceRadius, width, height);
                for (auto &&pixel : furnace)
                    if (map[pixel.getY()][pixel.getX()] != BLUE && map[pixel.getY()][pixel.getX()] != RED)
                        map[pixel.getY()][pixel.getX()] = MAGENTA;
            }
            if (map[y][x] == GREEN)
            {
                Position firefighter(x, y);
                std::vector<Position> action_area = circle_to_positions(firefighter, actionRadius, width, height);
                for (auto &&p : action_area)
                    if (map[p.getY()][p.getX()] == YELLOW || map[p.getY()][p.getX()] == ORANGE)
                        map[p.getY()][p.getX()] = LIME;
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
            float x_r = fireCenters[f].getX() + 0.5 + furnaceRadius * cos(degrees * (M_PI / 180.0));
            float y_r = fireCenters[f].getY() + 0.5 + furnaceRadius * sin(degrees * (M_PI / 180.0));
            float slope = (y_r - (fireCenters[f].getY() + 0.5)) / (x_r - (fireCenters[f].getX() + 0.5));
            float intercept = y_r - slope * x_r;
            Position source = fireCenters[f]; // possible copy of data. Can be improved later
            direction dir;
            if (degrees > 90 && degrees <= 270)
                dir = LEFT;
            else
                dir = RIGHT;

            std::vector<Position> rayPath = calculate_ray_path(map, source, slope, intercept, dir);

            Position target = rayPath[rayPath.size() - 1];

            for (size_t i = 1; i < rayPath.size(); i++)
            {
                Position p = rayPath[i];
                if (map[p.getY()][p.getX()] == LIME || map[p.getY()][p.getX()] == GREEN || map[p.getY()][p.getX()] == BLACK)
                    break;
                if (map[p.getY()][p.getX()] == YELLOW)
                    map[p.getY()][p.getX()] = ORANGE;
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

    std::vector<Position> fireCenters;
    std::vector<std::vector<int>> feasibilityMap;
    std::vector<std::vector<std::vector<int>>> rayFightingMap;
    std::cout << "Start gathering data..." << std::endl;

    // We get the fire centers and a map overlay of places we can't put firefighters
    for (int y = 0; y < height; y++)
    {
        std::vector<int> feasibilityMapLine;
        std::vector<std::vector<int>> rayFightingMapLine;
        for (int x = 0; x < width; x++)
        {
            if (map[y][x] == YELLOW)
                feasibilityMapLine.push_back(1);
            else
                feasibilityMapLine.push_back(0); // to change if firefighters can be in cities
            if (map[y][x] == RED)
            {
                Position fire(x, y);
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
        std::vector<Position> furnace = circle_to_positions(fireCenters[f], furnaceRadius, width, height);
        for (auto &&pixel : furnace)
        {
            feasibilityMap[pixel.getY()][pixel.getX()] = 0;
        }
    }

    std::cout << "Finished gathering fire furnace areas and removed them from feasible placements!"
              << std::endl;

    std::vector<FireVertex> fireList;

    for (size_t f = 0; f < nbFires; f++)
    {
        for (size_t r = 0; r < nbRays; r++)
        {
            float degrees = r * 360.0 / nbRays;
            float x_r = fireCenters[f].getX() + 0.5 + furnaceRadius * cos(degrees * (M_PI / 180.0));
            float y_r = fireCenters[f].getY() + 0.5 + furnaceRadius * sin(degrees * (M_PI / 180.0));
            float slope = (y_r - (fireCenters[f].getY() + 0.5)) / (x_r - (fireCenters[f].getX() + 0.5));
            float intercept = y_r - slope * x_r;
            Position source = fireCenters[f]; // Possible copy of data. Can be improved later.
            direction dir;
            if (degrees > 90 && degrees <= 270)
                dir = LEFT;
            else
                dir = RIGHT;

            std::vector<Position> rayPath = calculate_ray_path(map, source, slope, intercept, dir);

            Position target = rayPath[rayPath.size() - 1];

            if (map[target.getY()][target.getX()] == BLACK) // ray is directed to a city
            {
                calculate_ray_neighborhood(feasibilityMap, rayPath, actionRadius, (int)fireList.size(), rayFightingMap);
                fireList.push_back(FireVertex(source, target, fireList.size(), fireList.size()));
            }
        }
    }

    std::cout << "Finished gathering ray paths and feasible fighter placements!"
              << std::endl;

    std::vector<FighterVertex> fighterList;
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
                    fighter.addFire(fireList[r]);
                fighterList.push_back(fighter);
                fighterID++;
            }
        }
    }

    std::cout << "Finished gathering data!\n"
              << std::endl;

    return Graph(fireList, fighterList, isReduced, addAdjacency);
}
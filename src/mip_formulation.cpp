#include "mip_formulation.hpp"
#include "utils.hpp"

std::vector<std::vector<int>> solve(std::vector<std::vector<int>> map, std::vector<float> config)
{
    int nb_rays = (int)config[0];
    float furnace_radius = config[1];
    float action_radius = config[2];

    int height = map.size();
    int width = map[0].size();

    std::vector<pixel> fire_centers;
    std::vector<std::vector<int>> feasibility_map;
    // We get the fire centers and a map overlay of places we can't put firefighters
    for (int y = 0; y < height; y++)
    {
        std::vector<int> line;
        for (int x = 0; x < width; x++)
        {
            if (map[y][x] == YELLOW)
                line.push_back(1);
            else
                line.push_back(0); // to changer if firefighters can be in cities
            if (map[y][x] == RED)  //
            {
                pixel fire;
                fire.x = x;
                fire.y = y;
                fire_centers.push_back(fire);
            }
        }
        feasibility_map.push_back(line);
    }

    int nb_fires = fire_centers.size();

    for (int f = 0; f < nb_fires; f++)
    {
        std::vector<pixel> furnace = circle_to_pixels(fire_centers[f], furnace_radius, width, height);
        for (auto &&pixel : furnace)
            feasibility_map[pixel.y][pixel.x] = 0;
    }

    std::vector<std::vector<ray>> fire_rays;

    for (int f = 0; f < nb_fires; f++)
    {
        std::vector<ray> rays;
        for (int r = 0; r < nb_rays; r++)
        {
            float degrees = r * 360.0 / nb_rays;
            float x_r = fire_centers[f].x + 0.5 + furnace_radius + cos(degrees * (M_PI / 180));
            float y_r = fire_centers[f].y + 0.5 + furnace_radius + sin(degrees * (M_PI / 180));
            ray ray;
            ray.slope = (y_r - fire_centers[f].y + 0.5) / (x_r - fire_centers[f].x + 0.5);
            ray.intercept = y_r - ray.slope * x_r;
            ray.source = fire_centers[f]; // possible copy of data. Can be improved later
            if (degrees >= 90 && degrees < 270)
                ray.dir = LEFT;
            else
                ray.dir = RIGHT;

            std::vector<pixel> ray_path = calculate_ray_path(map, ray);

            rays.push_back(ray);
        }
        fire_rays.push_back(rays);
    }

    /** TODO:
     * - calculer les pixels que traverse chaque raie,
     * - ne garder que les raies qui sont dirigés vers des villes,
     * - calculer les voisinages de chaque chemin de raie selon le rayon d'action:
     *  --> avoir tous les pixels où on peut placer un pompier et être sûr qu'on arrete une raie de feu
     * - mip
     **/

    return map;
}
#include "mip_formulation.hpp"

using namespace std;

enum direction
{
    LEFT = 0,
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


vector<pixel> circle_to_pixels(pixel center, float radius, int width, int height){
    // possibly switch to float center coordinates and cast (int) to get pixels
    int x_bound_a = (int) floor(center.x + 0.5 - radius);
    int x_bound_b = (int) ceil(center.x + 0.5 + radius);
    int y_bound_a = (int) floor(center.y + 0.5 - radius);
    int y_bound_b = (int) ceil(center.y + 0.5 + radius);

    vector<pixel> pixels;

    for (int y = y_bound_a; y < y_bound_b; y++){
        for (int x = x_bound_a; x < x_bound_b; x++){
            if (x == center.x || y == center.y ||
                (x < center.x && y > center.y &&    // North-West
                pow(x+1-(center.x+0.5), 2) + pow(y-(center.y+0.5), 2) <= pow(radius,2)) ||
                (x < center.x && y < center.y &&    // South-West
                pow(x+1-(center.x+0.5), 2) + pow(y+1-(center.y+0.5), 2) <= pow(radius,2)) ||
                (x > center.x && y < center.y &&    // South-East
                pow(x-(center.x+0.5), 2) + pow(y+1-(center.y+0.5), 2) <= pow(radius,2)) ||
                (x > center.x && y > center.y &&    // North-East
                pow(x-(center.x+0.5), 2) + pow(y-(center.y+0.5), 2) <= pow(radius,2))
            ){
                if (x >= 0 && x < width && y >= 0 && y < height){
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

vector<pixel> calculate_ray_path(vector<vector<int>> map, ray ray){
    vector<pixel> path;

    // To Do

    return path;
}

vector<vector<int>> solve(vector<vector<int>> map, vector<float> config)
{
    int nb_rays = (int) config[0];
    float furnace_radius = config[1];
    float action_radius = config[2];

    int height = map.size();
    int width = map[0].size();

    vector<pixel> fire_centers;

    for (int y = 0 ; y < height ; y++){
        for (int x = 0 ; x < width ; x++){
            if (map[y][x] == 0) //RED
            {
                pixel fire;
                fire.x = x;
                fire.y = y;
                fire_centers.push_back(fire);
            }
            
        }
    }

    int nb_fires = fire_centers.size();

    vector<vector<ray>> fire_rays;

    for (int f = 0; f < nb_fires; f++)
    {
        vector<ray> rays;
        for (int r = 0; r < nb_rays; r++)
        {
            float degrees = r * 360.0 / nb_rays;
            float x_r = fire_centers[f].x + 0.5 + furnace_radius + cos(degrees * (M_PI / 180));
            float y_r = fire_centers[f].y + 0.5 + furnace_radius + sin(degrees * (M_PI / 180));
            ray ray;
            ray.slope = (y_r - fire_centers[f].y+0.5) / (x_r - fire_centers[f].x+0.5);
            ray.intercept = y_r - ray.slope * x_r;
            ray.source = fire_centers[f];   //possible copy of data. Can be improved later
            if (degrees >= 90 && degrees < 270)
                ray.dir = LEFT;
            else
                ray.dir = RIGHT;
            
            vector<pixel> ray_path = calculate_ray_path(map, ray);

            rays.push_back(ray);
        }
        fire_rays.push_back(rays);
    }
    
    /** To Do:
     * - calculer les pixels que traverse chaque raie,
     * - ne garder que les raies qui sont dirigés vers des villes,
     * - calculer les voisinages de chaque chemin de raie selon le rayon d'action:
     *  --> avoir tous les pixels où on peut placer un pompier et être sûr qu'on arrete une raie de feu
     * - mip
     **/


    return map;
}
#include "mip_formulation.hpp"

using namespace std;

enum direction
{
    LEFT = 0,
    RIGHT = 1,
};

struct point
{
    int x;
    int y;
};

struct ray
{
    point source;
    float slope;
    float intercept;
    direction dir;
};


vector<vector<int>> solve(vector<vector<int>> map, vector<float> config)
{
    int nb_rays = (int) config[0];
    float furnace_radius = config[1];
    float action_radius = config[2];

    int height = map.size();
    int width = map[0].size();

    vector<point> fire_centers;

    for (int y = 0 ; y < height ; y++){
        for (int x = 0 ; x < width ; x++){
            if (map[y][x] == 0) //RED
            {
                point fire;
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
            float x_r = fire_centers[f].x + furnace_radius + cos(degrees * (M_PI / 180));
            float y_r = fire_centers[f].y + furnace_radius + sin(degrees * (M_PI / 180));
            ray ray;
            ray.slope = (y_r - fire_centers[f].y) / (x_r - fire_centers[f].x);
            ray.intercept = y_r - ray.slope * x_r;
            ray.source = fire_centers[f];   //possible copy of data. Can be improved later
            if (degrees >= 90 && degrees < 270)
                ray.dir = LEFT;
            else
                ray.dir = RIGHT;
            
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
#define _USE_MATH_DEFINES
#include "mip_formulation.hpp"
#include "utils.hpp"
using namespace std;

std::vector<std::vector<int>> solve(std::vector<std::vector<int>> map, std::vector<float> config)
{
    int nb_rays = (int)config[0];
    //int nb_rays = 25;
    float furnace_radius = config[1];
    float action_radius = config[2];

    size_t height = map.size();
    size_t width = map[0].size();

    std::vector<pixel> fire_centers;
    std::vector<std::vector<int>> feasibility_map;
    std::cout << "Start gathering data" << std::endl;
    // We get the fire centers and a map overlay of places we can't put firefighters
    for (size_t y = 0; y < height; y++)
    {
        std::vector<int> line;
        for (size_t x = 0; x < width; x++)
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

    size_t nb_fires = fire_centers.size();

    std::cout << "Finished gathering firecenters and partial feasible placement data" << std::endl;

    for (size_t f = 0; f < nb_fires; f++)
    {
        std::vector<pixel> furnace = circle_to_pixels(fire_centers[f], furnace_radius, width, height);
        for (auto &&pixel : furnace)
            feasibility_map[pixel.y][pixel.x] = 0;
    }

    std::cout << "Finished gathering fire furnace areas and removed them from feasible placements" << std::endl;

    std::vector<std::vector<ray>> fire_rays;
    std::vector<std::vector<std::vector<pixel>>> fire_ray_paths;
    std::vector<std::vector<pixel>> fatal_ray_neighborhoods;

    for (size_t f = 0; f < nb_fires; f++)
    {
        std::vector<ray> rays;
        std::vector<std::vector<pixel>> ray_paths;
        for (size_t r = 0; r < nb_rays; r++)
        {
            float degrees = r * 360.0 / nb_rays;
            //std::cout << degrees;  //display current ray degrees
            float x_r = fire_centers[f].x + 0.5 + furnace_radius * cos(degrees * (M_PI / 180.0));
            float y_r = fire_centers[f].y + 0.5 + furnace_radius * sin(degrees * (M_PI / 180.0));
            //std::cout << " 2(" << x_r << ", " << y_r << ")" << std::endl;
            ray ray;
            ray.slope = (y_r - (fire_centers[f].y + 0.5)) / (x_r - (fire_centers[f].x + 0.5));
            ray.intercept = y_r - ray.slope * x_r;
            ray.source = fire_centers[f]; // possible copy of data. Can be improved later
            if (degrees > 90 && degrees <= 270)
                ray.dir = LEFT;
            else
                ray.dir = RIGHT;

            //std::cout << "Ray " << r << ": y = " << ray.slope << " * x + " << ray.intercept << " from(" << ray.source.x << ", " << ray.source.y << ")";

            std::vector<pixel> ray_path = calculate_ray_path(map, ray);

            //for (size_t i = 1; i < ray_path.size()-1; i++)
            //    if(map[ray_path[i].y][ray_path[i].x] == YELLOW) 
            //        map[ray_path[i].y][ray_path[i].x] = ORANGE;

            ray.target = ray_path[ray_path.size() - 1];

            //std::cout << " to(" << ray.target.x << ", " << ray.target.y << ") in direction " << ray.dir << std::endl;  //display current ray

            if (map[ray.target.y][ray.target.x] == BLACK) //ray is directed to a city
            {
                std::vector<pixel> ray_neighborhood = calculate_ray_neighborhood(feasibility_map, ray_path, action_radius);
                fatal_ray_neighborhoods.push_back(ray_neighborhood);
            }
            rays.push_back(ray);
            ray_paths.push_back(ray_path);
        }
        fire_rays.push_back(rays);
        fire_ray_paths.push_back(ray_paths);
    }

    std::cout << "Finished calculating ray paths and neighborhoods" << std::endl;


    /** TODO:
     * Check why rays going upwards seem thinner for some reason
     * Update feasibility map to limit it to positions that cover rays
     * Further improve feasibility map to reduce symetries (multiple positions that stop the same group of rays)
     **/

    bool verbose = true;
    int nb_fatal_rays = fatal_ray_neighborhoods.size();

    GRBVar** x = nullptr;
    try
    {
        // --- Creation of the Gurobi environment ---
        if (verbose)
            cout << "--> Creating the Gurobi environment" << endl;
        GRBEnv env = GRBEnv(true);
        // env.set("LogFile", "mip1.log"); ///< prints the log in a file
        env.start();

        // --- Creation of the Gurobi model ---
        if (verbose)
            cout << "--> Creating the Gurobi model" << endl;
        GRBModel model = GRBModel(env);

        if (!verbose)
        {
            model.set(GRB_IntParam_OutputFlag, 0);
        }

        // --- Creation of the variables ---
        if (verbose)
            cout << "--> Creating the variables" << endl;

        x = new GRBVar * [height];

        for (size_t j = 0; j < height; ++j)
        {
            x[j] = new GRBVar [width];
            for (size_t i = 0; i < width; ++i)
            {
                if (feasibility_map[j][i] == 1)
                {
                    stringstream ss;
                    ss << "Pixel(" << i << ", " << j << ")";
                    x[j][i] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, ss.str());
                }
            }
        }

        // --- Creation of the objective function ---
        if (verbose)
            cout << "--> Creating the objective function" << endl;
        GRBLinExpr obj = 0;
        for (size_t j = 0; j < height; ++j)
        {
            for (size_t i = 0; i < width; ++i)
            {
                if (feasibility_map[j][i] == 1)
                {
                    obj += x[j][i];
                }
            }
        }
        model.setObjective(obj, GRB_MINIMIZE);

        // --- Creation of the constraints ---
        if (verbose)
            cout << "--> Creating the constraints" << endl;

        // Each ray must be in the action range of a firefigher
        for (size_t r = 0; r < nb_fatal_rays; ++r)
        {
            GRBLinExpr ray_cover = 0;

            for (pixel pixel : fatal_ray_neighborhoods[r])
            {
                ray_cover += x[pixel.y][pixel.x];
            }

            stringstream ss;
            ss << "Ray_cover(" << r << ")";
            model.addConstr(ray_cover == 1, ss.str());
        }

        // Optimize model
        // --- Solver configuration ---
        if (verbose)
            cout << "--> Configuring the solver" << endl;
        model.set(GRB_DoubleParam_TimeLimit, 600.0); //< sets the time limit (in seconds)
        model.set(GRB_IntParam_Threads, 3);          //< limits the solver to single thread usage

        // --- Solver launch ---
        if (verbose)
            cout << "--> Running the solver" << endl;
        model.optimize();
        // model.write("model.lp"); //< Writes the model in a file

        // --- Solver results retrieval ---
        if (verbose)
            cout << "--> Retrieving solver results " << endl;

        int status = model.get(GRB_IntAttr_Status);
        if (status == GRB_OPTIMAL || (status == GRB_TIME_LIMIT && model.get(GRB_IntAttr_SolCount) > 0))
        {
            // the solver has computed the optimal solution or a feasible solution (when the time limit is reached before proving optimality)
            if (verbose)
            {
                cout << "Success! (Status: " << status << ")" << endl; //< prints the solver status (see the gurobi documentation)
                cout << "--> Printing results " << endl;
            }

            cout << "Result: ";
            cout << "runtime = " << model.get(GRB_DoubleAttr_Runtime) << " sec; ";
            cout << "objective value = " << model.get(GRB_DoubleAttr_ObjVal) << endl; //< gets the value of the objective function for the best computed solution (optimal if no time limit)

            if (verbose)
            {
                for (size_t j = 0; j < height; ++j)
                {
                    for (size_t i = 0; i < width; ++i)
                    {
                        if (feasibility_map[j][i] == 1 && x[j][i].get(GRB_DoubleAttr_X) >= 0.5)
                        {
                            pixel firefighter;
                            firefighter.x = i;
                            firefighter.y = j;
                            vector<pixel> action_area = circle_to_pixels(firefighter, action_radius, width, height);
                            for (pixel p : action_area)
                                if (map[p.y][p.x] == YELLOW || map[p.y][p.x] == ORANGE)
                                    map[p.y][p.x] = LIME;
                            cout << "We place a firefigher at position (" << i << ", " << j << ")" << endl;
                            map[j][i] = GREEN;
                        }
                    }
                }
                for (vector<vector<pixel>> ray_paths : fire_ray_paths) {
                    for (vector<pixel> ray_path : ray_paths) {
                        for (size_t i = 1; i < ray_path.size()-1; i++)
                        {
                            pixel p = ray_path[i];
                            if (map[p.y][p.x] == LIME)
                                break;
                            if (map[p.y][p.x] == YELLOW)
                                map[p.y][p.x] = ORANGE;
                        }
                    }
                }
            }
            // model.write("solution.sol"); //< Writes the solution in a file
        }
        else
        {
            // the model is infeasible (maybe wrong) or the solver has reached the time limit without finding a feasible solution
            cerr << "Fail! (Status: " << status << ")" << endl; //< see status page in the Gurobi documentation
        }
    }
    catch (GRBException e)
    {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    }
    catch (...)
    {
        cout << "Exception during optimization" << endl;
    }

    for (size_t j = 0; j < height; ++j)
    {
        delete[] x[j];
    }
    delete[] x;

    return map;
}
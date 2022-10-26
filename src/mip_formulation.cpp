#define _USE_MATH_DEFINES
#include "mip_formulation.hpp"

const std::vector<std::vector<Color>> &solve(std::vector<std::vector<Color>> &map, const std::vector<float> &config)
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
                feasibilityMapLine.push_back(0);
            if (map[y][x] == RED)
            {
                pixel fire;
                fire.x = x;
                fire.y = y;
                fireCenters.push_back(fire);
            }
            std::vector<int> rayFightingPos;
            rayFightingMapLine.push_back(rayFightingPos);
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

    std::cout << "Finished gathering fire furnace areas and removed them from feasible placements!" << std::endl;

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

    std::cout << "Finished calculating ray paths and neighborhoods!" << std::endl;

    bool verbose = true;
    int nbFatalRays = fatalRayNeighborhoods.size();

    GRBVar **x = nullptr;
    try
    {
        // --- Creation of the Gurobi environment ---
        if (verbose)
            std::cout << "--> Creating the Gurobi environment" << std::endl;
        GRBEnv env = GRBEnv(true);
        env.start();

        // --- Creation of the Gurobi model ---
        if (verbose)
            std::cout << "--> Creating the Gurobi model" << std::endl;
        GRBModel model = GRBModel(env);

        if (!verbose)
        {
            model.set(GRB_IntParam_OutputFlag, 0);
        }

        // --- Creation of the variables ---
        if (verbose)
            std::cout << "--> Creating the variables" << std::endl;

        x = new GRBVar *[height];

        for (size_t j = 0; j < height; ++j)
        {
            x[j] = new GRBVar[width];
            for (size_t i = 0; i < width; ++i)
            {
                if (feasibilityMap[j][i] == 1)
                {
                    std::stringstream ss;
                    ss << "pixel(" << i << ", " << j << ")";
                    x[j][i] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, ss.str());
                }
            }
        }

        // --- Creation of the objective function ---
        if (verbose)
            std::cout << "--> Creating the objective function" << std::endl;
        GRBLinExpr obj = 0;
        for (size_t j = 0; j < height; ++j)
        {
            for (size_t i = 0; i < width; ++i)
            {
                if (feasibilityMap[j][i] == 1)
                {
                    obj += x[j][i];
                }
            }
        }
        model.setObjective(obj, GRB_MINIMIZE);

        // --- Creation of the constraints ---
        if (verbose)
            std::cout << "--> Creating the constraints" << std::endl;

        // Each ray must be in the action range of a firefigher
        for (size_t r = 0; r < nbFatalRays; ++r)
        {
            GRBLinExpr rayCover = 0;

            for (pixel pixel : fatalRayNeighborhoods[r])
            {
                rayCover += x[pixel.y][pixel.x];
            }

            std::stringstream ss;
            ss << "ray_cover(" << r << ")";
            model.addConstr(rayCover >= 1, ss.str());
        }

        // Optimize model
        // --- Solver configuration ---
        if (verbose)
            std::cout << "--> Configuring the solver" << std::endl;
        model.set(GRB_DoubleParam_TimeLimit, 600.0); //< sets the time limit (in seconds)
        model.set(GRB_IntParam_Threads, 3);          //< limits the solver to single thread usage

        // --- Solver launch ---
        if (verbose)
            std::cout << "--> Running the solver" << std::endl;
        model.optimize();

        // --- Solver results retrieval ---
        if (verbose)
            std::cout << "--> Retrieving solver results " << std::endl;

        int status = model.get(GRB_IntAttr_Status);
        if (status == GRB_OPTIMAL || (status == GRB_TIME_LIMIT && model.get(GRB_IntAttr_SolCount) > 0))
        {
            // the solver has computed the optimal solution or a feasible solution (when the time limit is reached before proving optimality)
            if (verbose)
            {
                std::cout << "Success! (Status: " << status << ")" << std::endl; //< prints the solver status (see the gurobi documentation)
                std::cout << "--> Printing results " << std::endl;
            }

            std::cout << "Result: ";
            std::cout << "runtime = " << model.get(GRB_DoubleAttr_Runtime) << " sec; ";
            std::cout << "objective value = " << model.get(GRB_DoubleAttr_ObjVal) << std::endl; //< gets the value of the objective function for the best computed solution (optimal if no time limit)

            if (verbose)
            {
                for (size_t j = 0; j < height; ++j)
                {
                    for (size_t i = 0; i < width; ++i)
                    {

                        if (feasibilityMap[j][i] == 1 && x[j][i].get(GRB_DoubleAttr_X) >= 0.5)
                        {
                            pixel firefighter;
                            firefighter.x = i;
                            firefighter.y = j;
                            std::vector<pixel> action_area = circle_to_pixels(firefighter, actionRadius, width, height);
                            for (pixel p : action_area)
                                if (map[p.y][p.x] == YELLOW || map[p.y][p.x] == ORANGE)
                                    map[p.y][p.x] = LIME;
                            std::cout << "We place a firefigher at position (" << i << ", " << j << ")" << std::endl;
                            map[j][i] = GREEN;
                        }
                    }
                }
                for (std::vector<std::vector<pixel>> rayPaths : fireRayPaths)
                {
                    for (std::vector<pixel> rays : rayPaths)
                    {
                        for (size_t i = 1; i < rays.size(); i++)
                        {
                            pixel p = rays[i];
                            if (map[p.y][p.x] == LIME || map[p.y][p.x] == GREEN || map[p.y][p.x] == BLACK)
                                break;
                            if (map[p.y][p.x] == YELLOW)
                                map[p.y][p.x] = ORANGE;
                        }
                    }
                }
            }
        }
        else
        {
            // the model is infeasible (maybe wrong) or the solver has reached the time limit without finding a feasible solution
            std::cerr << "Fail! (Status: " << status << ")" << std::endl; //< see status page in the Gurobi documentation
        }
    }
    catch (GRBException e)
    {
        std::cout << "Error code = " << e.getErrorCode() << std::endl;
        std::cout << e.getMessage() << std::endl;
    }
    catch (...)
    {
        std::cout << "Exception during optimization" << std::endl;
    }

    for (size_t j = 0; j < height; ++j)
    {
        delete[] x[j];
    }
    delete[] x;

    return map;
}

const std::vector<std::vector<Color>> &solve_using_graph(
    std::vector<std::vector<Color>> &map,
    const std::vector<float> &config)
{
    size_t height = map.size();
    size_t width = map[0].size();

    Graph graph = calculate_graph_data(map, config);
    std::vector<FireVertex> fireVertexList = graph.getFireVertexList();
    std::vector<FighterVertex> fighterVertexList = graph.getFigtherVertexList();
    size_t nbFatalRays = fireVertexList.size();
    size_t nbFirefighters = fighterVertexList.size();

    bool verbose = true;

    GRBVar *x = nullptr;
    try
    {
        // --- Creation of the Gurobi environment ---
        if (verbose)
            std::cout << "--> Creating the Gurobi environment" << std::endl;
        GRBEnv env = GRBEnv(true);
        env.start();

        // --- Creation of the Gurobi model ---
        if (verbose)
            std::cout << "--> Creating the Gurobi model" << std::endl;
        GRBModel model = GRBModel(env);

        if (!verbose)
        {
            model.set(GRB_IntParam_OutputFlag, 0);
        }

        // --- Creation of the variables ---
        if (verbose)
            std::cout << "--> Creating the variables" << std::endl;

        x = new GRBVar[nbFirefighters];

        for (size_t i = 0; i < nbFirefighters; ++i)
        {
            Position p = fighterVertexList[i].getPos();
            std::stringstream ss;
            ss << "Fighter(" << p.getX() << ", " << p.getY() << ")";
            x[i] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, ss.str());
        }

        // --- Creation of the objective function ---
        if (verbose)
            std::cout << "--> Creating the objective function" << std::endl;
        GRBLinExpr obj = 0;
        for (size_t i = 0; i < nbFirefighters; ++i)
            obj += x[i];

        model.setObjective(obj, GRB_MINIMIZE);

        // --- Creation of the constraints ---
        if (verbose)
            std::cout << "--> Creating the constraints" << std::endl;

        // Each ray must be in the action range of a firefigher
        for (FireVertex fireRay : fireVertexList)
        {
            GRBLinExpr rayCover = 0;
            for (size_t i = 0; i < nbFirefighters; ++i)
            {
                FighterVertex fighter = fighterVertexList[i];
                for (FireVertex coveredRay : fighter.getFireCovered())
                {
                    if (fireRay.getID() == coveredRay.getID())
                    {
                        rayCover += x[i];
                        break;
                    }
                }
            }
            std::stringstream ss;
            ss << "ray_cover(" << fireRay.getID() << ")";
            model.addConstr(rayCover >= 1, ss.str());
        }

        // Optimize model
        // --- Solver configuration ---
        if (verbose)
            std::cout << "--> Configuring the solver" << std::endl;
        model.set(GRB_DoubleParam_TimeLimit, 600.0); //< sets the time limit (in seconds)
        model.set(GRB_IntParam_Threads, 3);          //< limits the solver to single thread usage

        // --- Solver launch ---
        if (verbose)
            std::cout << "--> Running the solver" << std::endl;
        model.optimize();

        // --- Solver results retrieval ---
        if (verbose)
            std::cout << "--> Retrieving solver results " << std::endl;

        int status = model.get(GRB_IntAttr_Status);
        if (status == GRB_OPTIMAL || (status == GRB_TIME_LIMIT && model.get(GRB_IntAttr_SolCount) > 0))
        {
            // the solver has computed the optimal solution or a feasible solution (when the time limit is reached before proving optimality)
            if (verbose)
            {
                std::cout << "Success! (Status: " << status << ")" << std::endl; //< prints the solver status (see the gurobi documentation)
                std::cout << "--> Printing results\n"
                          << std::endl;
            }

            std::cout << "Result: ";
            std::cout << "runtime = " << model.get(GRB_DoubleAttr_Runtime) << " sec; ";
            std::cout << "objective value = " << model.get(GRB_DoubleAttr_ObjVal) << std::endl; //< gets the value of the objective function for the best computed solution (optimal if no time limit)

            if (verbose)
            {
                std::cout << std::endl;
                for (size_t i = 0; i < nbFirefighters; ++i)
                {
                    if (x[i].get(GRB_DoubleAttr_X) >= 0.5)
                    {
                        Position pos = fighterVertexList[i].getPos();
                        pixel firefighter;
                        firefighter.x = pos.getX();
                        firefighter.y = pos.getY();
                        std::cout << "We place a firefigher at position (" << firefighter.x << ", " << firefighter.y << ")" << std::endl;
                        map[firefighter.y][firefighter.x] = GREEN;
                    }
                }
            }
            std::cout << std::endl;
        }
        else
        {
            // the model is infeasible (maybe wrong) or the solver has reached the time limit without finding a feasible solution
            std::cerr << "Fail! (Status: " << status << ")" << std::endl; //< see status page in the Gurobi documentation
        }
    }
    catch (GRBException e)
    {
        std::cout << "Error code = " << e.getErrorCode() << std::endl;
        std::cout << e.getMessage() << std::endl;
    }
    catch (...)
    {
        std::cout << "Exception during optimization" << std::endl;
    }

    delete[] x;

    return map;
}

// TODO: create more functions to clean up the code lmao
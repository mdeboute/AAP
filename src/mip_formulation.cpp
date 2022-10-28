#define _USE_MATH_DEFINES
#include "mip_formulation.hpp"

const std::vector<FighterVertex> mip_solve(
    Graph &graph)
{
    std::vector<FireVertex> fireVertexList = graph.getFireVertexList();
    std::vector<FighterVertex> fighterVertexList = graph.getFigtherVertexList();
    size_t nbFatalRays = fireVertexList.size();
    size_t nbFirefighters = fighterVertexList.size();

    bool verbose = true;
    std::vector<FighterVertex> solution;

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
                if (fighter.stopFire(fireRay))
                    rayCover += x[i];
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

            std::cout << std::endl;
            for (size_t i = 0; i < nbFirefighters; ++i)
            {
                if (x[i].get(GRB_DoubleAttr_X) >= 0.5)
                {
                    solution.push_back(fighterVertexList[i]);
                    Position pos = fighterVertexList[i].getPos();
                    if (verbose)
                        std::cout << "We place a firefigher at position (" << pos.getX() << ", " << pos.getY() << ")" << std::endl;
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

    return solution;
}

// TODO: create more functions to clean up the code lmao
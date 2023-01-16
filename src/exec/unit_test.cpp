#include "unit_test.hpp"

#include <vector>
#include <string>
#include <cstring>

Graph genVal1(bool verbose)
{
    std::vector<FireVertex> fires;
    std::vector<FighterVertex> fighters;
    for (int i = 0; i < 4; ++i)
    {
        fires.push_back(FireVertex(Position(), Position(), i, i));
    }
    for (int i = 0; i < 6; ++i)
    {
        fighters.push_back(FighterVertex(Position(), i, i));
    }

    // a
    fighters[0].addFire(fires[0]);
    fighters[0].addFire(fires[1]);
    // b
    fighters[1].addFire(fires[1]);
    // c
    fighters[2].addFire(fires[2]);
    fighters[2].addFire(fires[3]);
    // d
    fighters[3].addFire(fires[1]);
    // e
    fighters[4].addFire(fires[2]);
    // f
    fighters[5].addFire(fires[0]);
    fighters[5].addFire(fires[3]);

    return Graph(fires, fighters, true, true, verbose);
}

bool cutUselessFightersTest(Graph g, bool verbose)
{
    std::vector<FighterVertex> figthers = g.getFigtherVertexList();
    std::vector<FireVertex> fires;
    int cpt = 0;
    for (FighterVertex f : figthers)
    {
        fires = f.getFireCovered();
        int id = f.getID();
        if (id == 1 || id == 3 || id == 4)
        {
            if (verbose)
            {
                const std::string msg = "[FAIL] cutUselessFighters: unexpected figther in the graph, id : " + std::to_string(id);
                std::perror(msg.c_str());
            }
        }
        if (id == 0 || id == 2 || id == 5)
        {
            cpt++;
        }
    }
    if (cpt != 3)
    {
        if (verbose)
            printf("[FAIL] cutUselessFighters: Not enough figther in the graph, find %d insted of 3", cpt);
        return false;
    }
    if (verbose)
        printf("[SUCCESS] cutUselessFighters\n");
    return true;
}

bool generateAdjacencyTest(Graph g, bool verbose)
{
    std::vector<FighterVertex> figthers = g.getFigtherVertexList();
    std::vector<FireVertex> fires;
    for (FighterVertex f : figthers)
    {
        fires = f.getFireCovered();
        int Index = f.getIndex();
        std::vector<FireVertex> cutFires = g.getFighterAdjacencyList(Index);
        if (cutFires.size() != 2)
        {
            if (verbose)
            {
                const std::string msg = "[FAIL] generateAdjacency: fire couth = " + std::to_string(cutFires.size());
                std::perror(msg.c_str());
            }
            return false;
        }
        if (f.getFireCovered() != cutFires)
        {
            if (verbose)
            {
                const std::string msg = "[FAIL] generateAdjacency: wrong adjacent fire ligne for figther: " + std::to_string(f.getID());
                std::perror(msg.c_str());
            }
            return false;
        }
    }
    if (verbose)
        printf("[SUCCESS] generateAdjacency\n");
    return true;
}

int main(int argc, char *argv[])
{
    bool verbose = false;
    if (argc > 1 && strcmp(argv[1], "-v") == 0)
    {
        verbose = true;
    }

    Graph g = genVal1(verbose);

    if (verbose)
    {
        printf("----- Graph: fires -----\n");
        std::vector<FireVertex> fires = g.getFireVertexList();
        for (FireVertex &f : fires)
        {
            std::cout << f << std::endl;
        }
    }
    if (verbose)
    {
        printf("----- Graph: fighters -----\n");
        std::vector<FighterVertex> fighters = g.getFigtherVertexList();
        for (FighterVertex &f : fighters)
        {
            std::cout << f << std::endl;
        }
    }
    if (verbose)
        printf("----- [TEST] Graph -----\n");
    bool cut = cutUselessFightersTest(g, verbose);
    bool gen = generateAdjacencyTest(g, verbose);
    if (cut && gen)
    {
        if (verbose)
            printf("----- [SUCCESS] Graph -----\n");
        return 0;
    }
    if (verbose)
        std::perror("----- [FAIL] Graph -----\n");
    return 1;
}
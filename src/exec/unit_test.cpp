#include "unit_test.hpp"
#include "Graph/Graph.hpp"

#include <vector>
#include <string>

Graph genVal1()
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

    return Graph(fires, fighters, true, true);
}

bool cutUselessFightersTest(Graph g)
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
            const std::string msg = "[FAIL] cutUselessFighters: unexpected figther in the graph, id : " + std::to_string(id);
            std::perror(msg.c_str());
        }
        if (id == 0 || id == 2 || id == 5)
        {
            cpt++;
        }
    }
    if (cpt != 3)
    {
        printf("[FAIL] cutUselessFighters: Not enough figther in the graph, find %d insted of 3", cpt);
        return false;
    }
    printf("[SUCCESS] cutUselessFighters\n");
    return true;
}
bool generateAdjacencyTest(Graph g)
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
            const std::string msg = "[FAIL] generateAdjacency: fire couth = " + std::to_string(cutFires.size());
            std::perror(msg.c_str());
            return false;
        }
        if (f.getFireCovered() != cutFires)
        {
            const std::string msg = "[FAIL] generateAdjacency: wrong adjacent fire ligne for figther: " + std::to_string(f.getID());
            std::perror(msg.c_str());
            return false;
        }
    }
    printf("[SUCCESS] generateAdjacency\n");
    return true;
}

int main()
{
    Graph g = genVal1();
    printf("----- Graph: fires -----\n");
    std::vector<FireVertex> fires = g.getFireVertexList();
    for (FireVertex f : fires)
    {
        std::cout << f << std::endl;
    }
    printf("----- Graph: fighters -----\n");
    std::vector<FighterVertex> fighters = g.getFigtherVertexList();
    for (FighterVertex f : fighters)
    {
        std::cout << f << std::endl;
    }
    printf("----- [TEST] Graph -----\n");
    bool cut = cutUselessFightersTest(g);
    bool gen = generateAdjacencyTest(g);
    if (cut && gen)
    {
        printf("----- [SUCCESS] Graph -----\n");
        return 1;
    }
    std::perror("----- [FAIL] Graph -----\n");
    return 0;
}
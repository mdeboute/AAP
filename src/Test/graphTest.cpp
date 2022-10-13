#include "graphTest.hpp"
#include "Graph.hpp"

#include <vector>

Graph genVal1(){
    std::vector<FireVertex> fires;
    std::vector<FighterVertex> fighters;
    for (int i = 0; i < 4; ++i){
        fires.push_back(FireVertex(Position(), Position(), i));
    }
    for (int i = 0; i < 6; ++i){
        fighters.push_back(FighterVertex(Position(),i));
    }

    //a
    fighters[0].addFire(fires[0]);
    fighters[0].addFire(fires[1]);
    //b
    fighters[1].addFire(fires[1]);
    //c
    fighters[2].addFire(fires[2]);
    fighters[2].addFire(fires[3]);
    //d
    fighters[3].addFire(fires[1]);
    //e
    fighters[4].addFire(fires[2]);
    //f
    fighters[5].addFire(fires[0]);
    fighters[5].addFire(fires[3]);

    return Graph(fires,fighters);
}

void cutUselessFightersTest(Graph g){
    std::vector<FighterVertex> figthers = g.getFigtherVertexTab();
    std::vector<FireVertex> fires;
    for(FighterVertex f : figthers){
        fires = f.getFireLignes();
        int id = f.getID();
        if(id == 1 || id == 3 || id == 4){
            perror("[FAIL] cutUselessFighters : Unexpected figther in the graph, id : " + id);
            return;
        }
    }
    printf("[SUCESS] cutUselessFighters");
    return;
}
void generateAdjacencyTest(Graph g){
    std::vector<FighterVertex> figthers = g.getFigtherVertexTab();
    std::vector<FireVertex> fires;
    for(FighterVertex f : figthers){
        fires = f.getFireLignes();
        int id = f.getID();
        std::vector<FireVertex> cutFires = g.getFighterAdjacencyList(id);
        if(cutFires.size() != 2){
            perror("[FAIL] generateAdjacency : fire couth = " + cutFires.size());
            return ;
        }
        if( f.getFireLignes() != cutFires){
            perror("[FAIL] generateAdjacency : wrong adjacent fire ligne for figther : " + id);
            return ;
        }
    }
    printf("[SUCESS] generateAdjacency");
    return ;
}

void mainGraphTest(){
    Graph g = genVal1();
    printf("----- [TEST] Graph -----");
    cutUselessFightersTest(g);
    generateAdjacencyTest(g);
    printf("[SUCESS] Graph");
}
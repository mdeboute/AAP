#ifndef GRAPH
#define GRAPH

#include "FireVertex.hpp"
#include "FighterVertex.hpp"
#include "Edge.hpp"

#include <vector>

class Graph
{
private:
    std::vector<FireVertex> fireTab;
    std::vector<FighterVertex> fighterTab;

    std::vector<std::vector<int>> adjacencyMatrix;

    // a list of adjacent FireVertex for each firefighter. stocking id (position in list).
    std::vector<std::vector<FireVertex>> fighterAdjacencyList;

    // a list of adjacent FighterVertex for each fire. stocking id (position in list).
    std::vector<std::vector<FighterVertex>> fireAdjacencyList;
    void cutUselessFighters();
    void generateAdjacency();

public:
    Graph();
    Graph(std::vector<FireVertex> fireTab,
          std::vector<FighterVertex> fighterTab,
          std::vector<std::vector<FireVertex>> fighterAdjacencyList,
          std::vector<std::vector<FighterVertex>> fireAdjacencyList);

    Graph(std::vector<FireVertex> fireTab,
          std::vector<FighterVertex> fighterTab);

    const std::vector<FighterVertex> getFireNeightborhood(int ID);
    const std::vector<FireVertex> getFighterNeightborhood(int ID);
    const int isAdjacent(int fighterID, int fireID);
    const std::vector<FireVertex> &getFireVertexTab();
    const FireVertex &getFireVertex(int id);
    const std::vector<FighterVertex> &getFigtherVertexTab();
    const FighterVertex &getFigtherVertex(int id);
    const std::vector<std::vector<FireVertex>> &getFighterAdjacencyList();
    const std::vector<FireVertex> &getFighterAdjacencyList(int id);
    const std::vector<std::vector<FighterVertex>> &getFireAdjacencyList();
    const std::vector<FighterVertex> &getFireAdjacencyList(int id);
    const int getNbFires() { return fireTab.size(); }
};

#endif
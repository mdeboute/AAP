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

    // a list of adjacent FireVertex for each firefighter. stocking id (position in list).
    std::vector<std::vector<int>> fighterAdjacencyList;

    // a list of adjacent FighterVertex for each fire. stocking id (position in list).
    std::vector<std::vector<int>> fireAdjacencyList;

public:
    Graph();
    Graph(std::vector<FireVertex> fireTab,
          std::vector<FighterVertex> fighterTab,
          std::vector<std::vector<int>> fighterAdjacencyList,
          std::vector<std::vector<int>> fireAdjacencyList);

    Graph(std::vector<FireVertex> fireTab,
          std::vector<FighterVertex> fighterTab,
          std::vector<Edge> edges);

    const std::vector<FireVertex> &getFireVertexTab();
    const FireVertex &getFireVertex(int id);
    const std::vector<FighterVertex> &getFigtherVertexTab();
    const FighterVertex &getFigtherVertex(int id);
    const std::vector<std::vector<int>> &getFighterAdjacencyList();
    const std::vector<int> &getFighterAdjacencyList(int id);
    const std::vector<std::vector<int>> &getFireAdjacencyList();
    const std::vector<int> &getFireAdjacencyList(int id);
};

#endif
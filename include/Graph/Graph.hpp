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

    int isAdjacent(int fighterID, int fireID) const;
    int getNbFires() const;

    FireVertex getFireVertex(int id) const;
    const FighterVertex &getFigtherVertex(int id) const;

    const std::vector<FireVertex> &getFireVertexTab() const;
    const std::vector<FighterVertex> &getFigtherVertexTab() const;

    const std::vector<std::vector<FireVertex>> &getFighterAdjacencyList() const;
    const std::vector<FireVertex> &getFighterAdjacencyList(int id) const;
    const std::vector<std::vector<FighterVertex>> &getFireAdjacencyList() const;
    const std::vector<FighterVertex> &getFireAdjacencyList(int id) const;

    const std::vector<FighterVertex>& getFireNeightborhood(int FireID) const;
    const std::vector<FireVertex>& getFighterNeightborhood(int FighterID) const;
};

#endif
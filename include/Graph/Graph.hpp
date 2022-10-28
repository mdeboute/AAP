#ifndef GRAPH
#define GRAPH

#include "FireVertex.hpp"
#include "FighterVertex.hpp"
#include "Edge.hpp"

#include <vector>

class Graph
{
private:
    std::vector<FireVertex> fireList;
    std::vector<FighterVertex> fighterList;

    std::vector<std::vector<int>> adjacencyMatrix;

    // a list of adjacent FireVertex for each fireFighter. stocking id (position in list).
    std::vector<std::vector<FireVertex>> fighterAdjacencyList;

    // a list of adjacent FighterVertex for each fire. stocking id (position in list).
    std::vector<std::vector<FighterVertex>> fireAdjacencyList;
    void cutUselessFighters();
    void generateAdjacency();

public:
    Graph();
    Graph(std::vector<FireVertex> fireList,
          std::vector<FighterVertex> fighterList,
          std::vector<std::vector<FireVertex>> fighterAdjacencyList,
          std::vector<std::vector<FighterVertex>> fireAdjacencyList);
    Graph(std::vector<FireVertex> fireList,
          std::vector<FighterVertex> fighterList, bool isReduced, bool addAdjacency);

    int isAdjacent(int fighterIndex, int fireIndex) const;
    int getNbFires() const;

    FireVertex getFireVertex(int index) const;
    const FighterVertex &getFigtherVertex(int index) const;

    const std::vector<FireVertex> &getFireVertexList() const;
    const std::vector<FighterVertex> &getFigtherVertexList() const;

    const std::vector<std::vector<FireVertex>> &getFighterAdjacencyList() const;
    const std::vector<FireVertex> &getFighterAdjacencyList(int index) const;
    const std::vector<std::vector<FighterVertex>> &getFireAdjacencyList() const;
    const std::vector<FighterVertex> &getFireAdjacencyList(int index) const;

    const std::vector<FighterVertex> &getFireNeightborhood(int fireIndex) const;
    const std::vector<FireVertex> &getFighterNeightborhood(int fighterIndex) const;
};

#endif
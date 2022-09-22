#ifndef GRAPH
#define GRAPH

#include "FireVertex.hpp"
#include "FireFighterVertex.hpp"
#include "Edge.hpp"

class Graph{
    private :
        FireVertex * fireTab;
        FighterVertex * fighterTab;
        Edge* edgeTab;

    public :
        Graph(){}
        Graph(FireVertex * fireTab, FighterVertex * fighterTab, Edge* edgeTab);
        FireVertex* getFireVertexTab();
        FighterVertex* getFigtherVertexTab();
        Edge* getEdgeTab();
};


#endif
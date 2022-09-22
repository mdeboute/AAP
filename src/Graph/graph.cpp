#include "../../include/Graph/graph.hpp"

Graph::Graph(FireVertex * fireTab, FighterVertex * fighterTab, Edge* edgeTab){
    this->fireTab=fireTab;
    this->edgeTab=edgeTab;
    this->fighterTab=fighterTab;
}
        
FireVertex* Graph::getFireVertexTab(){
    return this->fireTab;
}
        
FighterVertex* Graph::getFigtherVertexTab(){
    return this->fighterTab;
}

Edge* Graph::getEdgeTab(){
    return this->edgeTab;
}


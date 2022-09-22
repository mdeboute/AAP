#include "../../include/Graph/Vertex.hpp"

Vertex::Vertex(Position p, int id){
    this->p = p;
    this->id = id;
}

Position Vertex::getPos(){
    std::cout << &(p) << std::endl;
    return p;
}

int Vertex::getID(){
    return this->id;
}
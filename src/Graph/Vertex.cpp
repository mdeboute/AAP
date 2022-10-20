#include "Graph/Vertex.hpp"

Vertex::Vertex(Position p, int id)
{
    this->p = p;
    this->id = id;
}

Position Vertex::getPos() const
{
    return p;
}

int Vertex::getID() const
{
    return this->id;
}

void Vertex::setID(int id){
    this->id = id;
}
#include "Graph/Vertex.hpp"

Vertex::Vertex(Position p, int id, int index)
{
    this->p = p;
    this->id = id;
    this->index = index;
}

Position Vertex::getPos() const
{
    return p;
}

int Vertex::getID() const
{
    return this->id;
}

void Vertex::setID(int id)
{
    this->id = id;
}

int Vertex::getIndex() const
{
    return this->index;
}

void Vertex::setIndex(int index)
{
    this->index = index;
}
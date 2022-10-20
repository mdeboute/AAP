#include "Graph/FireVertex.hpp"

FireVertex::FireVertex() {}

FireVertex::FireVertex(Position fireCenter, Position collide, int id) : Vertex(collide, id)
{
    this->fireCenter = fireCenter;
}

Position FireVertex::getFireCenter() const
{
    return fireCenter;
}
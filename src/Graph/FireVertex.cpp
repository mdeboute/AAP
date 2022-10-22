#include "Graph/FireVertex.hpp"

FireVertex::FireVertex() {}

FireVertex::FireVertex(Position fireCenter, Position collide, int id, int index) : Vertex(collide, id, index)
{
    this->fireCenter = fireCenter;
}

Position FireVertex::getFireCenter() const
{
    return fireCenter;
}
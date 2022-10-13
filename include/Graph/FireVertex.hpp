#ifndef FIREVERTEX
#define FIREVERTEX

#include "Vertex.hpp"

class FireVertex : public Vertex
{
private:
    Position fireCenter;

public:
    FireVertex();
    FireVertex(Position fireCenter, Position collide, int id);
    Position getFireCenter();
};Vertex
Vertex&os, FireVertex v)
{
    os << "fire of id : " << v.getID() << " from " << v.getFireCenter() << " to " << v.getPos();
    return os;
}

inline bool operator==(FireVertex f1, FireVertex f2)
{
    return (f1.getPos() == f2.getPos()) &&
           (f1.getID() == f2.getID()) &&
           (f1.getFireCenter() == f2.getFireCenter());
}

#endif
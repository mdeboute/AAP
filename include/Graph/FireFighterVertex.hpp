#ifndef FIREFIGHTERVERTEX
#define FIREFIGHTERVERTEX

#include "Vertex.hpp"

class FighterVertex : public Vertex
{
public:
    FighterVertex();
    FighterVertex(Position p, int id);
};

inline std::ostream &operator<<(std::ostream &os, FighterVertex v)
{
    os << "vertex of id : " << v.getID() << " and position " << v.getPos();
    return os;
}

inline bool operator==(FighterVertex f1, FighterVertex f2)
{
    return (f1.getPos() == f2.getPos()) &&
           (f1.getID() == f2.getID());
}

#endif
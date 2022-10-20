#ifndef FIGHTERVERTEX
#define FIGHTERVERTEX

#include "FireVertex.hpp"
#include <vector>

class FighterVertex : public Vertex
{
private:
    std::vector<FireVertex> fire_covered;
    bool parssingDone = false;

public:
    FighterVertex();
    FighterVertex(Position p, int id);
    void doneParsing();
    void addFire(FireVertex f);
    FireVertex getFireAt(int index);
    std::vector<FireVertex> getFireCovered();
    bool containsFighter(FighterVertex f);
    bool stopFire(FireVertex f);
    int getFireCapacity();
};

inline std::ostream &operator<<(std::ostream &os, FighterVertex v)
{
    os << "Vertex of id : " << v.getID() << " at position : " << v.getPos() << " with " << v.getFireCovered().size() << " fire lines";
    return os;
}

inline bool operator==(FighterVertex f1, FighterVertex f2)
{
    return (f1.getPos() == f2.getPos()) &&
           (f1.getID() == f2.getID());
}

#endif
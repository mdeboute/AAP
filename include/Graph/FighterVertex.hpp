#ifndef FIGHTERVERTEX
#define FIGHTERVERTEX

#include "FireVertex.hpp"
#include <vector>

class FighterVertex : public Vertex
{
private:
    std::vector<FireVertex> fireLignes;
    bool parssingDone = false;

public:
    FighterVertex();
    FighterVertex(Position p, int id);
    void doneParssing();
    void addFire(FireVertex f);
    FireVertex getFireAt(int index);
    std::vector<FireVertex> getFireLignes();
    bool containsFighter(FighterVertex f);
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
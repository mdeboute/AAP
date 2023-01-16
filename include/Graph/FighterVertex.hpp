#ifndef FIGHTERVERTEX
#define FIGHTERVERTEX

#include "FireVertex.hpp"

#include <vector>

class FighterVertex : public Vertex
{
private:
    std::vector<FireVertex> fireCovered;
    bool parssingDone = false;

public:
    FighterVertex();
    FighterVertex(Position p, int id, int index);

    void doneParsing();
    void addFire(FireVertex f);

    FireVertex getFireAtIndex(int index) const;
    bool betterThan(FighterVertex f) const;
    int compareFighters(FighterVertex f) const;
    bool stopFire(FireVertex f) const;
    int getNbFireCovered() const;
    void print(int verbose = 0);

    const std::vector<FireVertex> &getFireCovered() const;
};

inline std::ostream &operator<<(std::ostream &os, FighterVertex v)
{
    os << "Fighter of id: " << v.getID() << " at position: " << v.getPos() << " with " << v.getFireCovered().size() << " fire lines: " << std::endl;
    for (const FireVertex &f : v.getFireCovered())
    {
        os << " - " << f << std::endl;
    }
    return os;
}

inline bool operator==(FighterVertex f1, FighterVertex f2)
{
    return (f1.getPos() == f2.getPos()) &&
           (f1.getID() == f2.getID());
}

inline bool operator!=(FighterVertex f1, FighterVertex f2)
{
    return !(f1 == f2);
}

#endif
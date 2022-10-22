#ifndef EDGE
#define EDGE

#include "FighterVertex.hpp"
#include "FireVertex.hpp"

class Edge
{
private:
    FireVertex fire;
    FighterVertex fighter;
    int id = -42;

public:
    Edge();
    Edge(FighterVertex fighter, FireVertex fire, int id);
    
    FireVertex getFireVertex() const;
    FighterVertex getFighterVertex() const;
    int getID() const;
};

#endif
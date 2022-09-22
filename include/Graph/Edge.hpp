#ifndef EDGE
#define EDGE

#include "FireFighterVertex.hpp"
#include "FireVertex.hpp"

class Edge{
    private :
        FireVertex fire;
        FighterVertex fighter;
        int id = -42;

    public :
        Edge();
        Edge(FighterVertex fighter, FireVertex fire, int id);
        FireVertex getFireVertex();
        FighterVertex getFighterVertex();
        int getID();



};

#endif
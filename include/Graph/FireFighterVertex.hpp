#ifndef FIREFIGHTERVERTEX
#define FIREFIGHTERVERTEX

#include "Vertex.hpp"

class FighterVertex : public Vertex {
    public :
        FighterVertex(){}
        FighterVertex(Position p, int id) : Vertex(p,id){};
};

#endif
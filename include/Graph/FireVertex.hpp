#ifndef FIREVERTEX
#define FIREVERTEX

#include "Vertex.hpp"

class FireVertex : public Vertex {
    private :
        Position fireCenter;
    public :
        FireVertex(){}
        FireVertex(Position p, int id) : Vertex(p,id){}
        Position getFireCenter(){return fireCenter;}
};

#endif
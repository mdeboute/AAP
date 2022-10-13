#ifndef VERTEX
#define VERTEX

#include "Position.hpp"

// abstract class
class Vertex
{
private:
    Position p;
    int id = -42;

protected:
    Vertex() {}
    Vertex(Position p, int id);

public:
    Position getPos();
    int getID();
    void setID(int id){this->id = id;}
};

#endif
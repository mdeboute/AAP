#ifndef VERTEX
#define VERTEX

#include "Position.hpp"

// abstract class
class Vertex
{
private:
    Position p;
    int index = -1;
    int id = -42;

protected:
    Vertex() {}
    Vertex(Position p, int id, int index);

public:
    Position getPos() const;
    int getID() const;
    int getIndex() const;

    void setID(int id);
    void setIndex(int index);
};

#endif
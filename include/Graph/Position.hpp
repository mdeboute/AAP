#ifndef POSITION
#define POSITION

#include <iostream>

class Position{
    private :
        int x = -42;
        int y = -42;

    public :
        int getX();
        int getY();
        Position(){}
        Position(int x,int y);
        double dist(Position p);
};

inline std::ostream& operator << (std::ostream& os, Position p){
    os << "(" << p.getX() << ";" << p.getY() << ")";
    return os;
}

inline bool operator == (Position p1, Position p2){
    return (
        p1.getX()==p2.getX() &&
        p1.getY()==p2.getY());
}

#endif
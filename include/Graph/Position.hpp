#ifndef POSITION
#define POSITION

#include <iostream>

class Position
{
private:
    double x = -42;
    double y = -42;

public:
    Position() {}
    Position(double x, double y);

    double getX() const;
    double getY() const;

    double dist(Position p) const;
};

inline std::ostream &operator<<(std::ostream &os, Position p)
{
    os << "(" << p.getX() << ";" << p.getY() << ")";
    return os;
}

inline bool operator==(Position p1, Position p2)
{
    return (
        p1.getX() == p2.getX() &&
        p1.getY() == p2.getY());
}

#endif
#include "Graph/Position.hpp"
#include <cmath>

Position::Position(double x, double y)
{
    this->x = x;
    this->y = y;
}

double Position::getX()
{
    return this->x;
}

double Position::getY()
{
    return this->y;
}

// euclidian distance
double Position::dist(Position p)
{
    int xDelta = this->x - p.getX();
    int yDelta = this->y - p.getY();
    return std::sqrt((xDelta * xDelta) + (yDelta * yDelta));
}
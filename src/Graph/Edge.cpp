#include "Graph/Edge.hpp"

Edge::Edge() {}

Edge::Edge(FighterVertex fighter, FireVertex fire, int id)
{
    this->fighter = fighter;
    this->fire = fire;
    this->id = id;
}

FireVertex Edge::getFireVertex() const
{
    return fire;
}

FighterVertex Edge::getFighterVertex() const
{
    return fighter;
}

int Edge::getID() const
{
    return id;
}
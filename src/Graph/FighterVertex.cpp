#include "Graph/FighterVertex.hpp"

FighterVertex::FighterVertex() {}

FighterVertex::FighterVertex(Position p, int id, int index) : Vertex(p, id, index) {}

void FighterVertex::doneParsing()
{
    parssingDone = true;
}

void FighterVertex::addFire(FireVertex f)
{
    if (parssingDone)
        return;
    fireCovered.push_back(f);
}

FireVertex FighterVertex::getFireAt(int index) const
{
    return fireCovered[index];
}

const std::vector<FireVertex> &FighterVertex::getFireCovered() const
{
    return fireCovered;
}

bool FighterVertex::betterThan(FighterVertex f) const
{
    std::vector<FireVertex> fireLines = f.getFireCovered();
    for (FireVertex fire : fireLines)
    {
        bool find = false;
        for (FireVertex myFire : fireCovered)
        {
            if (fire == myFire)
            {
                find = true;
                break;
            }
        }
        if (!find)
        {
            return false;
        }
    }
    return true;
}

bool FighterVertex::stopFire(FireVertex f) const
{
    for (FireVertex fire : fireCovered)
    {
        if (fire == f)
        {
            return true;
        }
    }
    return false;
}

int FighterVertex::getFireCapacity() const
{
    return fireCovered.size();
}

void FighterVertex::print(int verbose)
{
    if (verbose == 1)
    {
        std::cout << *this << std::endl;
    }
    else
    {
        std::cout << "Vertex of id: " << this->getID() << " at position: " << this->getPos() << std::endl;
    }
}
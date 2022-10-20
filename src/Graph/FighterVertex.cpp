#include "Graph/FighterVertex.hpp"

FighterVertex::FighterVertex() {}

FighterVertex::FighterVertex(Position p, int id) : Vertex(p, id) {}

void FighterVertex::doneParsing()
{
    parssingDone = true;
}

void FighterVertex::addFire(FireVertex f)
{
    if (parssingDone)
        return;
    fire_covered.push_back(f);
}

FireVertex FighterVertex::getFireAt(int index) const
{
    return fire_covered[index];
}

const std::vector<FireVertex>& FighterVertex::getFireCovered() const
{
    return fire_covered;
}
/*
true : tous les feu de f sont contenus dans la liste
false : un des feux de f n'est pas dans la liste
*/
bool FighterVertex::containsFighter(FighterVertex f) const
{
    std::vector<FireVertex> fLignes = f.getFireCovered();
    for (FireVertex fire : fLignes)
    {
        bool find = false;
        for (FireVertex myFire : fire_covered)
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
    for (FireVertex fire : fire_covered)
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
    return fire_covered.size();
}
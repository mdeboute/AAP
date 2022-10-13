#include "Graph/FighterVertex.hpp"

FighterVertex::FighterVertex() {}

FighterVertex::FighterVertex(Position p, int id) : Vertex(p, id) {}

void FighterVertex::doneParssing()
{
    parssingDone = true;
}

void FighterVertex::addFire(FireVertex f)
{
    if (parssingDone)
        return;
    fireLignes.push_back(f);
}

FireVertex FighterVertex::getFireAt(int index)
{
    return fireLignes[index];
}

std::vector<FireVertex> FighterVertex::getFireLignes()
{
    return fireLignes;
}
/*
true : tous les feu de f sont contenus dans la liste
false : un des feux de f n'est pas dans la liste
*/
bool FighterVertex::containsFighter(FighterVertex f)
{
    std::vector<FireVertex> fLignes = f.getFireLignes();
    for (FireVertex fire : fLignes)
    {
        bool find = false;
        for (FireVertex myFire : fireLignes)
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
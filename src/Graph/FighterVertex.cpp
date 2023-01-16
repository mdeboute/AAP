#include "Graph/FighterVertex.hpp"
#include "utils.hpp"

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

FireVertex FighterVertex::getFireAtIndex(int index) const
{
    return fireCovered[index];
}

const std::vector<FireVertex> &FighterVertex::getFireCovered() const
{
    return fireCovered;
}

int FighterVertex::compareFighters(FighterVertex f) const
{
    int bestFighter = -1;
    std::vector<int> fireIDs;
    int m = f.getFireCovered().size();

    for (size_t i = 0; i < m; i++)
        fireIDs.push_back(f.getFireCovered()[i].getID());

    int nbCommonFiresCovered = 0;
    for (const FireVertex &fire : fireCovered)
        if (fire.getID() >= fireIDs[0] && fire.getID() <= fireIDs[m - 1] && dichotomic_search(fireIDs, 0, m, fire.getID()) == -1)
            nbCommonFiresCovered++;

    if (nbCommonFiresCovered == m)
        bestFighter = getID();
    else if (nbCommonFiresCovered == fireCovered.size())
        bestFighter = f.getID();

    return bestFighter;
}

bool FighterVertex::stopFire(FireVertex f) const
{
    for (const FireVertex &fire : fireCovered)
    {
        if (fire == f)
        {
            return true;
        }
    }
    return false;
}

int FighterVertex::getNbFireCovered() const
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
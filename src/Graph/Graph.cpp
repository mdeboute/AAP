#include "Graph/Graph.hpp"
#include <chrono>

Graph::Graph() {}

Graph::Graph(std::vector<FireVertex> fireList,
             std::vector<FighterVertex> fighterList,
             std::vector<std::vector<FireVertex>> fighterAdjacencyList,
             std::vector<std::vector<FighterVertex>> fireAdjacencyList)
{
    this->fireList = fireList;
    this->fighterList = fighterList;
    this->fighterAdjacencyList = fighterAdjacencyList;
    this->fireAdjacencyList = fireAdjacencyList;
}

Graph::Graph(std::vector<FireVertex> fireTab,
             std::vector<FighterVertex> fighterTab, bool isReduced, bool addAdjacency, bool verbose)
{
    this->fireList = fireTab;
    this->fighterList = fighterTab;
    if (isReduced)
        cutUselessFighters(verbose);
    if (addAdjacency)
        generateAdjacency();
}

void Graph::cutUselessFighters(bool verbose)
{   
    if (verbose)
        std::cout << "Start cutting useless fighters... (starting with " << fighterList.size() << " fighters)" << std::endl;
    auto startingTime = std::chrono::steady_clock::now();

    std::vector<FighterVertex> usefullFighters;
    for (FighterVertex fighter : fighterList)
    {
        bool usefullness = (fighter.getFireCovered().size() != 0);
        if (usefullness)
        {
            for (int i = 0; i < usefullFighters.size(); i++)
            {
                FighterVertex usefulFighter = usefullFighters[i];
                int bestFighterId = usefulFighter.compareFighters(fighter);
                if (bestFighterId == usefulFighter.getID()) {
                    usefullness = false;
                    break;
                }
                else if (bestFighterId == fighter.getID())
                {
                    usefullFighters.erase(usefullFighters.begin() + i);
                    i--;
                }
            }
        }
        if (usefullness)
            usefullFighters.push_back(fighter);
    }
    this->fighterList = usefullFighters;
    for (int i = 0; i < fighterList.size(); ++i)
        fighterList[i].setIndex(i);

    std::chrono::duration<double> tt = std::chrono::steady_clock::now() - startingTime;
    if (verbose)
        std::cout << "Finished cutting useless fighters in " << tt.count() << " sec! (" << fighterList.size() << " remainings)\n"
                  << std::endl;
}

const std::vector<FighterVertex> &Graph::getFireNeightborhood(int index) const
{
    return this->fireAdjacencyList.at(index);
}

const std::vector<FireVertex> &Graph::getFighterNeightborhood(int index) const
{
    return this->fighterAdjacencyList.at(index);
}

int Graph::isAdjacent(int fighterIndex, int fireIndex) const
{
    return adjacencyMatrix.at(fighterIndex).at(fireIndex);
}

const std::vector<FireVertex> &Graph::getFireVertexList() const
{
    return fireList;
}

FireVertex Graph::getFireVertex(int index) const
{
    return fireList.at(index);
}

const std::vector<FighterVertex> &Graph::getFigtherVertexList() const
{
    return fighterList;
}

const FighterVertex &Graph::getFigtherVertex(int index) const
{
    return fighterList.at(index);
}

const std::vector<std::vector<FireVertex>> &Graph::getFighterAdjacencyList() const
{
    return fighterAdjacencyList;
}

const std::vector<FireVertex> &Graph::getFighterAdjacencyList(int index) const
{
    return fighterAdjacencyList.at(index);
}

const std::vector<std::vector<FighterVertex>> &Graph::getFireAdjacencyList() const
{
    return fireAdjacencyList;
}

const std::vector<FighterVertex> &Graph::getFireAdjacencyList(int index) const
{
    return fireAdjacencyList.at(index);
}

void Graph::generateAdjacency()
{
    // init matrix and lists:
    for (int i = 0; i < fighterList.size(); ++i)
    {
        adjacencyMatrix.push_back(std::vector<int>());
        fighterAdjacencyList.push_back(std::vector<FireVertex>());
        for (int j = 0; j < fireList.size(); ++j)
        {
            adjacencyMatrix[i].push_back(0);
            if (i == 0)
            {
                fireAdjacencyList.push_back(std::vector<FighterVertex>());
            }
        }
    }

    for (FighterVertex figther : fighterList)
    {
        std::vector<FireVertex> fires = figther.getFireCovered();
        int figtherIndex = figther.getIndex();
        for (FireVertex fire : fires)
        {
            int fireIndex = fire.getIndex();
            adjacencyMatrix[figtherIndex][fireIndex] = 1;
            fireAdjacencyList[fireIndex].push_back(figther);
            fighterAdjacencyList[figtherIndex].push_back(fire);
        }
    }
}
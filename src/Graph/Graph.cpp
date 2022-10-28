#include "Graph/Graph.hpp"

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
             std::vector<FighterVertex> fighterTab, bool isReduced, bool addAdjacency)
{
    this->fireList = fireTab;
    this->fighterList = fighterTab;
    if (isReduced)
        cutUselessFighters();
    if (addAdjacency)
        generateAdjacency();
    // std::cout << "After cut, only " << fighterTab.size() << "pos of fighters left" << std::endl;
}

void Graph::cutUselessFighters()
{
    std::vector<FighterVertex> usefullFighters;
    std::cout << "Old Nb Fighters: " << fighterList.size() << std::endl;
    for (FighterVertex fighter : fighterList)
    {
        bool usefullness = (fighter.getFireCovered().size() != 0);
        if (usefullness)
        {
            for (int i = 0; i < usefullFighters.size(); i++)
            {
                FighterVertex usefulFighter = usefullFighters[i];
                if (fighter.betterThan(usefulFighter))
                {
                    usefullFighters.erase(usefullFighters.begin() + i);
                    i--;
                }
                else if (usefulFighter.betterThan(fighter))
                {
                    usefullness = false;
                    break;
                }
            }
        }
        if (usefullness)
        {
            usefullFighters.push_back(fighter);
        }
    }
    this->fighterList = usefullFighters;
    std::cout << "New Nb Fighters: " << fighterList.size() << std::endl << std::endl;
    for (int i = 0; i < fighterList.size(); ++i)
    {
        fighterList[i].setIndex(i);
        //fighterList[i].print(1);
    }
    for (int i = 0; i < fireList.size(); ++i)
    {
        fireList[i].setIndex(i);
    }
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
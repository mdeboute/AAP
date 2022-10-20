#include "Graph/Graph.hpp"

Graph::Graph() {}

Graph::Graph(std::vector<FireVertex> fireTab,
             std::vector<FighterVertex> fighterTab,
             std::vector<std::vector<FireVertex>> fighterAdjacencyList,
             std::vector<std::vector<FighterVertex>> fireAdjacencyList)
{
    this->fireTab = fireTab;
    this->fighterTab = fighterTab;
    this->fighterAdjacencyList = fighterAdjacencyList;
    this->fireAdjacencyList = fireAdjacencyList;
}

Graph::Graph(std::vector<FireVertex> fireTab,
             std::vector<FighterVertex> fighterTab)
{
    this->fireTab = fireTab;
    this->fighterTab = fighterTab;
    cutUselessFighters();
    generateAdjacency();
    // std::cout << "After cut, only " << fighterTab.size() << "pos of fighters left" << std::endl;
}

void Graph::cutUselessFighters()
{
    std::vector<FighterVertex> usefullFighters;

    for (FighterVertex f : this->fighterTab)
    {
        int fUsefullness = (f.getFireCovered().size() != 0);

        if (fUsefullness)
        {
            for (int i = 0; i < usefullFighters.size(); i++)
            {
                FighterVertex f2 = usefullFighters[i];
                if (fUsefullness)
                {
                    if (f2.containsFighter(f))
                    {
                        fUsefullness = 0;
                    }
                    else
                    {
                        if (f.containsFighter(f2))
                        {
                            usefullFighters.erase(usefullFighters.begin() + i);
                            usefullFighters.push_back(f);
                            fUsefullness = 0;
                        }
                    }
                }
            }
        }
        if (fUsefullness)
        {
            usefullFighters.push_back(f);
        }
    }
    this->fighterTab = usefullFighters;
    for (int i = 0; i < fighterTab.size(); ++i)
    {
        fighterTab[i].setIndex(i);
    }
    for (int i = 0; i < fireTab.size(); ++i)
    {
        fireTab[i].setIndex(i);
    }
}

const std::vector<FighterVertex>& Graph::getFireNeightborhood(int Index) const
{
    return this->fireAdjacencyList.at(Index);
}

const std::vector<FireVertex>& Graph::getFighterNeightborhood(int Index) const
{
    return this->fighterAdjacencyList.at(Index);
}

int Graph::isAdjacent(int fighterIndex, int fireIndex) const
{
    return adjacencyMatrix.at(fighterIndex).at(fireIndex);
}

const std::vector<FireVertex>& Graph::getFireVertexTab() const
{
    return fireTab;
}

FireVertex Graph::getFireVertex(int Index) const
{
    return fireTab.at(Index);
}

const std::vector<FighterVertex>& Graph::getFigtherVertexTab() const
{
    return fighterTab;
}

const FighterVertex& Graph::getFigtherVertex(int Index) const
{
    return fighterTab.at(Index);
}

const std::vector<std::vector<FireVertex>>& Graph::getFighterAdjacencyList() const
{
    return fighterAdjacencyList;
}

const std::vector<FireVertex> &Graph::getFighterAdjacencyList(int Index) const
{
    return fighterAdjacencyList.at(Index);
}

const std::vector<std::vector<FighterVertex>>& Graph::getFireAdjacencyList() const
{
    return fireAdjacencyList;
}

const std::vector<FighterVertex>& Graph::getFireAdjacencyList(int Index) const
{
    return fireAdjacencyList.at(Index);
}

void Graph::generateAdjacency()
{
    // init matrix and lists:
    for (int i = 0; i < fighterTab.size(); ++i)
    {
        adjacencyMatrix.push_back(std::vector<int>());
        fighterAdjacencyList.push_back(std::vector<FireVertex>());
        for (int j = 0; j < fireTab.size(); ++j)
        {
            adjacencyMatrix[i].push_back(0);
            if (i == 0)
            {
                fireAdjacencyList.push_back(std::vector<FighterVertex>());
            }
        }
    }

    for (FighterVertex figther : fighterTab)
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
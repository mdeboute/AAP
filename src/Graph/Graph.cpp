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
    std::cout << "after cut, only " << fighterTab.size() << "pos of fighters left" << std::endl;
}

void Graph::cutUselessFighters()
{
    std::vector<FighterVertex> usefullFighters;

    for (FighterVertex f : this->fighterTab)
    {
        int fUsefullness = (f.getFireLines().size() != 0);

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
        fighterTab[i].setID(i);
    }
    for (int i = 0; i < fireTab.size(); ++i)
    {
        fireTab[i].setID(i);
    }
}

const std::vector<FighterVertex>& Graph::getFireNeightborhood(int ID) const
{
    return this->fireAdjacencyList.at(ID);
}

const std::vector<FireVertex>& Graph::getFighterNeightborhood(int ID) const
{
    return this->fighterAdjacencyList.at(ID);
}

int Graph::isAdjacent(int fighterID, int fireID) const
{
    return adjacencyMatrix.at(fighterID).at(fireID);
}

const std::vector<FireVertex>& Graph::getFireVertexTab() const
{
    return fireTab;
}

FireVertex Graph::getFireVertex(int id) const
{
    return fireTab.at(id);
}

const std::vector<FighterVertex>& Graph::getFigtherVertexTab() const
{
    return fighterTab;
}

const FighterVertex& Graph::getFigtherVertex(int id) const
{
    return fighterTab.at(id);
}

const std::vector<std::vector<FireVertex>>& Graph::getFighterAdjacencyList() const
{
    return fighterAdjacencyList;
}

const std::vector<FireVertex> &Graph::getFighterAdjacencyList(int id) const
{
    return fighterAdjacencyList.at(id);
}

const std::vector<std::vector<FighterVertex>>& Graph::getFireAdjacencyList() const
{
    return fireAdjacencyList;
}

const std::vector<FighterVertex>& Graph::getFireAdjacencyList(int id) const
{
    return fireAdjacencyList.at(id);
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
        std::vector<FireVertex> fires = figther.getFireLines();
        int figtherID = figther.getID();
        for (FireVertex fire : fires)
        {
            int fireId = fire.getID();
            adjacencyMatrix[figtherID][fireId] = 1;
            fireAdjacencyList[fireId].push_back(figther);
            fighterAdjacencyList[figtherID].push_back(fire);
        }
    }
}
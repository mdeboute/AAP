#include "Graph/Graph.hpp"

Graph::Graph() {}

Graph::Graph(std::vector<FireVertex> fireTab,
             std::vector<FighterVertex> fighterTab,
             std::vector<std::vector<int>> fighterAdjacencyList,
             std::vector<std::vector<int>> fireAdjacencyList)
{
    this->fireTab = fireTab;
    this->fighterTab = fighterTab;
    this->fighterAdjacencyList = fighterAdjacencyList;
    this->fireAdjacencyList = fireAdjacencyList;
}

Graph::Graph(std::vector<FireVertex> fireTab,
             std::vector<FighterVertex> fighterTab,
             std::vector<Edge> edges)
{
    this->fireTab = fireTab;
    this->fighterTab = fighterTab;
    for (int i = 0; i < fireTab.size(); i++)
    {
        this->fireAdjacencyList.push_back(std::vector<int>());
    }

    for (int i = 0; i < fighterTab.size(); i++)
    {
        this->fighterAdjacencyList.push_back(std::vector<int>());
    }

    for (Edge e : edges)
    {
        int fireID = e.getFireVertex().getID();
        int fighterID = e.getFighterVertex().getID();

        if (fireID >= fireTab.size() || fighterID >= fighterTab.size())
        {
            std::cout << "!!! in graph creation, edge " << e.getID() << " given have fire and fighter id of " << fireID
                      << " and " << fighterID << " while max id are " << fireTab.size() << " and " << fighterTab.size() << " !!! " << std::endl;
        }

        this->fighterAdjacencyList[fighterID].push_back(fireID);
        this->fireAdjacencyList[fireID].push_back(fighterID);
    }
}

const std::vector<FireVertex> &Graph::getFireVertexTab()
{
    return fireTab;
}

const FireVertex &Graph::getFireVertex(int id)
{
    return fireTab[id];
}

const std::vector<FighterVertex> &Graph::getFigtherVertexTab()
{
    return fighterTab;
}

const FighterVertex &Graph::getFigtherVertex(int id)
{
    return fighterTab[id];
}

const std::vector<std::vector<int>> &Graph::getFighterAdjacencyList()
{
    return fighterAdjacencyList;
}

const std::vector<int> &Graph::getFighterAdjacencyList(int id)
{
    return fighterAdjacencyList[id];
}

const std::vector<std::vector<int>> &Graph::getFireAdjacencyList()
{
    return fireAdjacencyList;
}

const std::vector<int> &Graph::getFireAdjacencyList(int id)
{
    return fireAdjacencyList[id];
}
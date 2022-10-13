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
    cutUselessFighters();
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

void Graph::cutUselessFighters()
{
    std::vector<FighterVertex> usefullFighters;

    for (FighterVertex f : this->fighterTab)
    {
        int fUsefullness = (f.getFireLignes().size() != 0);

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
Graph::Graph(std::vector<FireVertex> fireTab,
            std::vector<FighterVertex> fighterTab){
    this->fireTab = fireTab;
    this->fighterTab = fighterTab;
    cutUselessFighters();
    generateAdjacencyMatrix();
}

const std::vector<FighterVertex> Graph::getFireNeightborhood(int ID){
    //TO DO
}
const std::vector<FireVertex> Graph::getFighterNeightborhood(int ID){
    //TO DO
}
const int Graph::isAdjacent(int fireID, int fighterID){
    //TO DO
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

const std::vector<std::vector<FireVertex>> &Graph::getFighterAdjacencyList()
{
    return fighterAdjacencyList;
}

const std::vector<FireVertex> &Graph::getFighterAdjacencyList(int id)
{
    return fighterAdjacencyList[id];
}

const std::vector<std::vector<FighterVertex>> &Graph::getFireAdjacencyList()
{
    return fireAdjacencyList;
}

const std::vector<FighterVertex> &Graph::getFireAdjacencyList(int id)
{
    return fireAdjacencyList[id];
}

void Graph::generateAdjacency(){
    //init matrix and lists: 
    for(int i = 0; i < fighterTab.size() ; ++i){
        AdjacencyMatrix.push_back(std::vector<int>());
        fighterAdjacencyList.push_back(std::vector<FireVertex>());
        for(int j = 0; j < fireTab.size() ; ++j){
            AdjacencyMatrix[i].push_back(0);
            if (i == 0){
               fireAdjacencyList.push_back(std::vector<FighterVertex>()); 
            }
        }
    }
    
    for(FighterVertex figther : fighterTab){
        std::vector<FireVertex> fires = figther.getFireLignes();
        int figtherID = figther.getID();
        for (FireVertex fire : fires){
            int fireId = fire.getID();
            AdjacencyMatrix[figtherID][fireId] = 1;
            fireAdjacencyList[fireId].push_back(figther);
            fighterAdjacencyList[figtherID].push_back(fire);
        }
    }
}
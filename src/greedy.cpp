#include "greedy.hpp"
#include <chrono>

std::vector<FighterVertex> greedySolve(Graph& graph){
    auto startingTime = std::chrono::steady_clock::now();
    std::vector<FighterVertex> Sol;
    std::vector<FighterVertex> fighters = graph.getFigtherVertexTab();
    if (fighters.size()==0)std::cout << "invalid input in greedy !!!" << std::endl;
    std::vector<FireVertex> fires = graph.getFireVertexTab();
    std::vector<int> isFireCovered;
    std::vector<int> fightersValue;
    for (int i=0; i<fires.size(); ++i){
        isFireCovered.push_back(0);
    }
    for (int i=0; i<fighters.size(); ++i){
        fightersValue.push_back(0);
    }

    int allFireCovered = 0;
    while(allFireCovered==0){
        allFireCovered = 1;

        //compute value of fighters
        for (int i=0; i<fightersValue.size(); ++i){
            if (fightersValue[i]>=0){
                fightersValue[i] = 0;
                std::vector<FireVertex> fighterFires = fighters[i].getFireLines();
                for (int j=0; j<fighterFires.size(); ++j){
                    if (isFireCovered[fighterFires[j].getID()]==0){
                        fightersValue[i]++;
                    }
                }
            }
        }

        int bestFighter = -1;
        int bestValue = -1;
        for (int i=0; i<fightersValue.size(); i++){
            if (fightersValue[i]>bestValue){
                bestValue = fightersValue[i];
                bestFighter = i;
            }
        }

        std::cout << "took fighter " << bestFighter << std::endl;

        fightersValue[bestFighter] = -1;
        Sol.push_back(fighters[bestFighter]);
        std::vector<FireVertex> fighterFires = fighters[bestFighter].getFireLines();
        for (int j=0; j<fighterFires.size(); ++j){
            isFireCovered[fighterFires[j].getID()] = 1;
        }

        for (int i=0; i<fires.size(); ++i){
            if (allFireCovered==1){
                if (isFireCovered[i]==0){
                    allFireCovered = 0;
                }
            }
        }
    }
    std::chrono::duration<double> tt = std::chrono::steady_clock::now() - startingTime;
    std::cout << "solved with greedy in " << tt.count() << "sec" << std::endl;

    return Sol;
}
#include "Graph/FireFighterVertex.hpp"

FighterVertex::FighterVertex() {}

FighterVertex::FighterVertex(Position p, int id) : Vertex(p, id) {}

void FighterVertex::doneParssing(){
    parssingDone = true;
}

void FighterVertex::addFire(FireVertex f){
    if (parssingDone) return;
    fireLignes.push_back(f);
}

FireVertex FighterVertex::getFireAt(int index){
    return fireLignes[index];
}
    
std::vector<FireVertex> FighterVertex::getFireLignes(){
    return fireLignes;
}
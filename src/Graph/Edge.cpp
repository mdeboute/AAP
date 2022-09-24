#include "../../include/Graph/Edge.hpp"

Edge::Edge(){}

Edge::Edge(FighterVertex fighter, FireVertex fire, int id){
    this->fighter = fighter;
    this->fire = fire;
    this->id = id;
}

FireVertex Edge::getFireVertex(){
    return fire;
}

FighterVertex Edge::getFighterVertex(){
    return fighter;
}

int Edge::getID(){
    return id;
}
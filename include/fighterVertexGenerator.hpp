#ifndef FIGHTERVERTEXGENERATOR
#define FIGHTERVERTEXGENERATOR

#include "Graph/Graph.hpp"

// generate all the fighterVertex
std::vector<FighterVertex> generateFighter(std::vector<FireVertex> fireTab);

// return all the fighterVertex that are concerned by "fire" and members of "subfireTab"
// best fited in fireVertex class ??
std::vector<FighterVertex> generateFightersOfFire(std::vector<FireVertex> subFireTab, FireVertex fire);

#endif
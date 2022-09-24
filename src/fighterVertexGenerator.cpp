#include "../include/fighterVertexGenerator.hpp"

// call generateFighterOfFire for all fire, 
// note that if a call as been made on a specific fire,
// all fighter that fix this fire are already generated.
// so we call generateFighterOfFire on the set of not yet treated fires

std::vector<FighterVertex> generateFighter(std::vector<FireVertex> fireTab){
    std::vector<FighterVertex> fighterTab;
    for (int i=0; i<fireTab.size(); i++){
        std::vector<FighterVertex> fighterOfI;
        std::vector<FireVertex> subFires;
        for (int j=i+1; j<fireTab.size(); j++){
            subFires.push_back(fireTab[j]);
        }
        fighterOfI = generateFightersOfFire(subFires,fireTab[i]);
        for (FighterVertex f : fighterOfI){
            fighterTab.push_back(f);
        }
    }
    return fighterTab;
}

std::vector<FighterVertex> generateFightersOfFire(std::vector<FireVertex> subFireTab, FireVertex fire){
    /* The algorithm

        we first define 2 new lines that are parrallel to "fire" and have a distance to 2*r to "fire"
        (with r the radius of firefighter protection)

        if a line of fire (call her "intersectFire") intersect with one of thoose lines, we can define the position that are middleway between
        "fire" and the intersection point, the position should cover "fire" and "intersectFire".

        now at some point later, "intersectFire" and the other parrallel line will intersect, that point will be the maximum
        point at wich we can fix "fire" and "intersectFire" with one fighter

        we can then create a graph of interval between the fire center and the fire collision of "fire". with every set of 
        intervals that are superposed, we can try to solve a system of equation to find the point (if it exist) that fix them
        all. 

        when we have treated all the set of intervalls that are superposed, we have found all the fighter positions that fix
        "fire" and potentially some others.
    */
    return std::vector<FighterVertex>();
}
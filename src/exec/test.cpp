#include "Graph/FighterVertex.hpp"
#include "Graph/FireVertex.hpp"
#include "utils.hpp"

#include <vector>

int main()
{
    Position p(5, 6);
    FighterVertex f(p, 1);
    FighterVertex f2(p, 2);
    FighterVertex f25(p, 3);
    FireVertex f3(p, p, 12);
    FireVertex f6(p, p, 13);
    FireVertex f5(p, p, 14);
    FireVertex f4(p, p, 15);

    std::cout << "f : " << f.getFireLignes().size() << std::endl;
    f.addFire(f3);
    f.addFire(f4);
    std::cout << "f : " << f.getFireLignes().size() << std::endl;

    f2.addFire(f3);
    f2.addFire(f4);
    f2.addFire(f5);
    std::cout << "f2 : " << f2.getFireLignes().size() << std::endl;

    f25.addFire(f3);
    f25.addFire(f4);
    f25.addFire(f5);
    f25.addFire(f6);
    std::cout << "f25 : " << f25.getFireLignes().size() << std::endl;

    std::vector<FighterVertex> fighters;
    fighters.push_back(f);
    fighters.push_back(f2);
    fighters.push_back(f25);

    std::cout << "here" << std::endl;
    for (int i = 0; i < fighters.size(); i++)
    {
        std::cout << fighters[i] << std::endl
                  << std::endl;
    }
}
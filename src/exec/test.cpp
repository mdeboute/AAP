#include "Graph/FireFighterVertex.hpp"
#include "Graph/FireVertex.hpp"

#include <vector>

int main()
{
    Position p(5, 6);
    FighterVertex f(p, 12);
    FighterVertex f2(p, 12);
    FireVertex f3(p, p, 12);
    FighterVertex f4(p, 15);

    std::cout << f << std::endl;
    std::cout << f3 << std::endl;
    std::cout << (f == f2) << " " << (f == f4) << std::endl;
}
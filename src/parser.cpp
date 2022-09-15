#include <iostream>
#include "parser.hpp"

void openFile(std::ifstream &file, std::string filePath)
{
    file.open(filePath);
    if (file.is_open())
    {
        std::cout << "File opened successfully" << std::endl;
    }
    else
    {
        std::cerr << "File open failed\n"
                  << std::endl;
        exit(-1);
    }
}

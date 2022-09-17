#include <iostream>
#include "parser.hpp"


int getColorCode(int r, int g, int b){
    if (r==255 && g==0 && b==0)
        return 0;
    else if (r==0 && g==255 && b==0)
        return 1;
    else if (r==0 && g==0 && b==0)
        return 2;
    else if (r==0 && g==0 && b==255)
        return 3;
    else if (r==255 && g==255 && b==0)
        return 4;
    return -1;
}

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

std::vector<std::vector<int>> processMapFile(std::ifstream &file)
{
    // data format:
    std::string line;
    std::vector<std::vector<int>> map;
    std::vector<int> row;
    char const delimiter = ' ';

    // skip the 2 first lines
    for (int i = 0; i < 2; i++)
    {
        std::getline(file, line);
    }

    std::getline(file, line);
    int w = std::stoi(line.substr(0, line.find(delimiter)));
    line.erase(0, line.find(delimiter) + 1);
    int h = std::stoi(line.substr(0, line.find(delimiter)));

    // skip the 4th line
    std::getline(file, line);

    for (int y = 0; y < h; y++){
        for (int x = 0; x < w; x++)
        {
            std::getline(file, line);
            int r = std::stoi(line.substr(0, line.find(delimiter)));
            line.erase(0, line.find(delimiter) + 1);
            int g = std::stoi(line.substr(0, line.find(delimiter)));
            line.erase(0, line.find(delimiter) + 1);
            int b = std::stoi(line.substr(0, line.find(delimiter)));

            row.push_back(getColorCode(r,g,b));
        }
        map.push_back(row);
        row.clear();
    }
    return map;
}

std::vector<std::vector<int>> parseMap(std::string filePath)
{
    std::ifstream file;
    openFile(file, filePath);
    std::vector<std::vector<int>> map = processMapFile(file);
    file.close();
    return map;
}

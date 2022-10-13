#ifndef FILE_IO_HPP
#define FILE_IO_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "Graph/FighterVertex.hpp"

enum Color
{
    RED = 0,     // color to mark fire center
    GREEN = 1,   // color to mark firefighter position
    BLACK = 2,   // color to mark city area
    BLUE = 3,    // color to mark inaccessible area
    YELLOW = 4,  // color to visualize ray paths
    ORANGE = 5,  // color to visualize ray paths
    MAGENTA = 6, // color to visualize fire furnace areas
    LIME = 7,    // color to visualize firefigter action areas
    CYAN = 8,    // color to visualize ray neighborhoods
    ERROR = -1
};

void openFile(std::ifstream &file, std::string filePath);
std::vector<std::vector<Color>> processMapFile(std::ifstream &file);
std::vector<std::vector<Color>> parseMap(std::string filePath);
std::vector<float> processConfig(std::ifstream &filePath);
std::vector<float> parseConfig(std::string filePath);
void writeMap(const std::string &filePath, std::vector<std::vector<Color>> map);

#endif
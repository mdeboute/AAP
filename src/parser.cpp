#include <iostream>
#include "parser.hpp"

int getColorCode(int r, int g, int b)
{
    if (r == 255 && g == 0 && b == 0)
        return RED;
    else if (r == 0 && g == 0 && b == 0)
        return BLACK;
    else if (r == 0 && g == 0 && b == 255)
        return BLUE;
    else if (r == 255 && g == 255 && b == 0)
        return YELLOW;
    return -1;
}

void getCodeColor(Color code, int* r, int* g, int* b) {
    switch (code)
    {
    case RED:
        *r = 255;
        *g = 0;
        *b = 0;
        break;
    case GREEN:
        *r = 0;
        *g = 255;
        *b = 0;
        break;
    case BLACK:
        *r = 0;
        *g = 0;
        *b = 0;
        break;
    case BLUE:
        *r = 0;
        *g = 0;
        *b = 255;
        break;
    case YELLOW:
        *r = 255;
        *g = 255;
        *b = 0;
        break;
    case ORANGE:
        *r = 255;
        *g = 165;
        *b = 0;
        break;
    case MAGENTA:
        *r = 255;
        *g = 0;
        *b = 255;
        break;
    case LIME:
        *r = 165;
        *g = 255;
        *b = 0;
        break;
    case CYAN:
        *r = 0;
        *g = 255;
        *b = 255;
        break;
    default:
        break;
    }
}

void openFile(std::ifstream &file, std::string filePath)
{
    file.open(filePath);
    if (!file.is_open())
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

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            std::getline(file, line);
            int r = std::stoi(line.substr(0, line.find(delimiter)));
            line.erase(0, line.find(delimiter) + 1);
            int g = std::stoi(line.substr(0, line.find(delimiter)));
            line.erase(0, line.find(delimiter) + 1);
            int b = std::stoi(line.substr(0, line.find(delimiter)));

            row.push_back(getColorCode(r, g, b));
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

std::vector<float> processConfig(std::ifstream &filePath)
{
    std::vector<float> config;
    std::string line;
    char const delimiter = ' ';

    for (int i = 0; i < 3; i++)
    {
        std::getline(filePath, line);
        config.push_back(std::stof(line.substr(0, line.find(delimiter))));
    }
    return config;
}

std::vector<float> parseConfig(std::string filePath)
{
    std::ifstream file;
    openFile(file, filePath);
    std::vector<float> config = processConfig(file);
    file.close();
    return config;
}

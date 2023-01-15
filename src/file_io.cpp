#include <iostream>
#include "file_io.hpp"
#include "Graph/FighterVertex.hpp"
#include "utils.hpp"

Color getColorCode(int r, int g, int b)
{
    if (r == 255 && g == 0 && b == 0)
        return RED;
    else if (r == 0 && g == 0 && b == 0)
        return BLACK;
    else if (r == 0 && g == 0 && b == 255)
        return BLUE;
    else if (r == 255 && g == 255 && b == 0)
        return YELLOW;
    return ERROR;
}

void getColorCode(Color code, int *r, int *g, int *b)
{
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

void open_file(std::ifstream &file, std::string filePath)
{
    file.open(filePath);
    if (!file.is_open())
    {
        std::cerr << filePath << " open failed!\n"
                  << std::endl;
        exit(-1);
    }
}

std::vector<std::vector<Color>> process_map_file(std::ifstream &file)
{
    // data format:
    std::string line;
    std::vector<std::vector<Color>> map;
    std::vector<Color> row;
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

std::vector<std::vector<Color>> parse_map(std::string filePath)
{
    std::ifstream file;
    open_file(file, filePath);
    std::vector<std::vector<Color>> map = process_map_file(file);
    file.close();
    return map;
}

std::vector<float> process_config(std::ifstream &filePath)
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

std::vector<float> parse_config(std::string filePath)
{
    std::ifstream file;
    open_file(file, filePath);
    std::vector<float> config = process_config(file);
    file.close();
    return config;
}

void write_map(const std::string &filePath, std::vector<std::vector<Color>> map)
{
    std::ofstream file;
    file.open(filePath);

    size_t height = map.size();
    size_t width = map[0].size();

    file << "P3" << std::endl;
    file << width << " " << height << std::endl;
    file << "255" << std::endl;

    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            int r, g, b;
            getColorCode(map[y][x], &r, &g, &b);
            file << r << " " << g << " " << b << std::endl;
        }
    }

    file.close();
}

void write_solution(const std::string &filePath, std::vector<std::vector<Color>> map, std::vector<float> config, std::vector<FighterVertex> solution)
{
    for (FighterVertex fighter : solution)
    {
        Position pos = fighter.getPos();
        map[pos.getY()][pos.getX()] = GREEN;
    }

    map = draw_details(map, config);
    write_map(filePath, map);
}
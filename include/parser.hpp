#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

void openFile(std::ifstream& file, std::string filePath);
std::vector<std::vector<int>> processMapFile(std::ifstream& file);
std::vector<std::vector<int>> parseMap(std::string filePath);

#endif
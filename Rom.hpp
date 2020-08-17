//
// Created by lorenzo on 8/16/20.
//

#ifndef ZURGBEMU_ROM_HPP
#define ZURGBEMU_ROM_HPP
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "Memory.hpp"

std::vector<uint8_t> LoadRomFromFile(std::string fileName);
#endif //ZURGBEMU_ROM_HPP

//
// Created by lorenzo on 8/16/20.
//
#include "Rom.hpp"

std::vector<uint8_t> LoadRomFromFile(std::string fileName)
{
    std::ifstream rom(fileName, std::ios::in | std::ios::binary);

    // skip newlines
    rom.unsetf(std::ios::skipws);


    if(!rom.is_open())
    {
        return std::vector<uint8_t>(0);
    }
    else
    {
        return std::vector<uint8_t>((std::istreambuf_iterator<char>(rom)),
                                 std::istreambuf_iterator<char>());
    }
}

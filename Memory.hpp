//
// Created by lorenzo on 8/16/20.
//

#ifndef ZURGBEMU_MEMORY_HPP
#define ZURGBEMU_MEMORY_HPP
#include <cstdint>
#include <cassert>
#include <array>
#include <vector>
#include <algorithm>
#include <iterator>

const uint16_t MAX_GAMEBOY_MEMORY = 0xFFFF;

class Memory
{
public:
                                Memory();
                                ~Memory();
    uint8_t                     Read(uint16_t address); // 16 bit words
    void                        Write(uint16_t address, uint8_t data);
    void                        SetRomIntoMemory(const std::vector<uint8_t>& rom);
    void                        Reset();

private:
    std::array<uint8_t, MAX_GAMEBOY_MEMORY> bytes;
};

#endif //ZURGBEMU_MEMORY_HPP

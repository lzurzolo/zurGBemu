#include "Memory.hpp"
#include "CPU.hpp"
#include "Rom.hpp"

int main()
{
    CPU cpu;
    Memory memory;
    auto romData = LoadRomFromFile("tetris.gb");
    if(romData.empty())
    {
        std::cout << "Failed to load rom" << std::endl;
    }
    else
    {
        cpu.Reset();
        memory.Reset();
        memory.SetRomIntoMemory(romData);

        for(uint16_t i = 0x0000; i < 0x8000; i++)
        {
            std::cout << memory.Read(i) << std::endl;
        }
    }
}

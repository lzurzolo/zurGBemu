#include "Memory.hpp"
#include "CPU.hpp"
#include "Rom.hpp"

int main()
{
    Memory memory;
    CPU cpu{&memory};
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
        while(1) cpu.Step();
    }
}

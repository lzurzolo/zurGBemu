//
// Created by lorenzo on 8/16/20.
//

#ifndef ZURGBEMU_CPU_HPP
#define ZURGBEMU_CPU_HPP
#include <cstdint>
#include <iostream>
#include "Memory.hpp"

struct Registers
{
    uint8_t a;
    uint8_t f;

    uint8_t b;
    uint8_t c;

    uint8_t d;
    uint8_t e;

    uint8_t h;
    uint8_t l;

    uint16_t sp;
    uint16_t pc;
};

class CPU
{
public:
    CPU(Memory* m);
    ~CPU();
    void Reset();
    void Step();

private:
    void HandleInstruction(uint8_t instruction);
    void NOP();
    void LDrr(uint8_t& r1, uint8_t& r2);
    void LDrr(uint8_t& r1, uint8_t& r2msb, uint8_t& r2lsb);


    Registers registers{};
    Memory* memory;
};


#endif //ZURGBEMU_CPU_HPP

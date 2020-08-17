//
// Created by lorenzo on 8/16/20.
//

#ifndef ZURGBEMU_CPU_HPP
#define ZURGBEMU_CPU_HPP
#include <cstdint>

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
    CPU();
    ~CPU();
    void Reset();
private:
    Registers registers{};
};


#endif //ZURGBEMU_CPU_HPP

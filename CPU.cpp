//
// Created by lorenzo on 8/16/20.
//

#include "CPU.hpp"

CPU::CPU(Memory* m)
: memory(m)
{
    Reset();
}

CPU::~CPU()
= default;

void CPU::Reset()
{
    registers.a = 0x01;
    registers.f = 0xb0;
    registers.b = 0x00;
    registers.c = 0x13;
    registers.d = 0x00;
    registers.e = 0xD8;
    registers.h = 0x01;
    registers.l = 0x4d;
    registers.sp = 0xFFFE;
    registers.pc = 0x100;
}

void CPU::Step()
{
    HandleInstruction(memory->Read(registers.pc++));
}

void CPU::HandleInstruction(uint8_t instruction)
{
    if(instruction == 0x00) NOP();

    else if(instruction == 0x06) LDrn(registers.b);
    else if(instruction == 0x0E) LDrn(registers.c);
    else if(instruction == 0x16) LDrn(registers.d);
    else if(instruction == 0x1E) LDrn(registers.e);
    else if(instruction == 0x26) LDrn(registers.h);
    else if(instruction == 0x2E) LDrn(registers.l);

    else if(instruction == 0x7F) LDrr(registers.a, registers.a);
    else if(instruction == 0x78) LDrr(registers.a, registers.b);
    else if(instruction == 0x79) LDrr(registers.a, registers.c);
    else if(instruction == 0x7A) LDrr(registers.a, registers.d);
    else if(instruction == 0x7B) LDrr(registers.a, registers.e);
    else if(instruction == 0x7C) LDrr(registers.a, registers.h);
    else if(instruction == 0x7D) LDrr(registers.a, registers.l);
    else if(instruction == 0x7E) LDrHL(registers.a)
    else if(instruction == 0x40) LDrr(registers.b, registers.b);
    else if(instruction == 0x41) LDrr(registers.b, registers.c);
    else if(instruction == 0x42) LDrr(registers.b, registers.d);
    else if(instruction == 0x43) LDrr(registers.b, registers.e);
    else if(instruction == 0x44) LDrr(registers.b, registers.h);
    else if(instruction == 0x45) LDrr(registers.b, registers.l);
    else if(instruction == 0x46) LDrHL(registers.b)
    else if(instruction == 0x48) LDrr(registers.c, registers.b);
    else if(instruction == 0x49) LDrr(registers.c, registers.c);
    else if(instruction == 0x4A) LDrr(registers.c, registers.d);
    else if(instruction == 0x4B) LDrr(registers.c, registers.e);
    else if(instruction == 0x4C) LDrr(registers.c, registers.h);
    else if(instruction == 0x4D) LDrr(registers.c, registers.l);
    else if(instruction == 0x4E) LDrHL(registers.c)
    else if(instruction == 0x50) LDrr(registers.d, registers.b);
    else if(instruction == 0x51) LDrr(registers.d, registers.c);
    else if(instruction == 0x52) LDrr(registers.d, registers.d);
    else if(instruction == 0x53) LDrr(registers.d, registers.e);
    else if(instruction == 0x54) LDrr(registers.d, registers.h);
    else if(instruction == 0x55) LDrr(registers.d, registers.l);
    else if(instruction == 0x56) LDrHL(registers.d)
    else if(instruction == 0x58) LDrr(registers.e, registers.b);
    else if(instruction == 0x59) LDrr(registers.e, registers.c);
    else if(instruction == 0x5A) LDrr(registers.e, registers.d);
    else if(instruction == 0x5B) LDrr(registers.e, registers.e);
    else if(instruction == 0x5C) LDrr(registers.e, registers.h);
    else if(instruction == 0x5D) LDrr(registers.e, registers.l);
    else if(instruction == 0x5E) LDrHL(registers.e)
    else if(instruction == 0x60) LDrr(registers.h, registers.b);
    else if(instruction == 0x61) LDrr(registers.h, registers.c);
    else if(instruction == 0x62) LDrr(registers.h, registers.d);
    else if(instruction == 0x63) LDrr(registers.h, registers.e);
    else if(instruction == 0x64) LDrr(registers.h, registers.h);
    else if(instruction == 0x65) LDrr(registers.h, registers.l);
    else if(instruction == 0x66) LDrHL(registers.h)
    else if(instruction == 0x68) LDrr(registers.l, registers.b);
    else if(instruction == 0x69) LDrr(registers.l, registers.c);
    else if(instruction == 0x6A) LDrr(registers.l, registers.d);
    else if(instruction == 0x6B) LDrr(registers.l, registers.e);
    else if(instruction == 0x6C) LDrr(registers.l, registers.h);
    else if(instruction == 0x6D) LDrr(registers.l, registers.l);
    else if(instruction == 0x6E) LDrHL(registers.l);
    else if(instruction == 0x70) LDHLr(registers.b);
    else if(instruction == 0x71) LDHLr(registers.c);
    else if(instruction == 0x72) LDHLr(registers.b);
    else if(instruction == 0x73) LDHLr(registers.e);
    else if(instruction == 0x74) LDHLr(registers.h);
    else if(instruction == 0x75) LDHLr(registers.l);
    else if(instruction == 0x36)
    {
        uint8_t data = memory->Read(registers.pc++);
        LDHLr(data);
    }
}

void CPU::NOP()
{

}

void CPU::LDrn(uint8_t &r)
{
    r = memory->Read(registers.pc);
}

void CPU::LDrr(uint8_t& r1, uint8_t& r2)
{
    r1 = r2;
}

void CPU::LDHLr(uint8_t &r)
{
    uint16_t hl = (uint16_t)(registers.h << 8) | registers.l;
    memory->Write(hl, r);
}

void CPU::LDrHL(uint8_t &r)
{
    uint16_t hl = (uint16_t)(registers.h << 8) | registers.l;
    uint8_t data = memory->Read(hl);
    r = data;
}






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
    else if(instruction == 0x7F) LDrr(registers.a, registers.a);
    else if(instruction == 0x78) LDrr(registers.a, registers.b);
    else if(instruction == 0x79) LDrr(registers.a, registers.c);
    else if(instruction == 0x7A) LDrr(registers.a, registers.d);
    else if(instruction == 0x7B) LDrr(registers.a, registers.e);
    else if(instruction == 0x7C) LDrr(registers.a, registers.h);
    else if(instruction == 0x7D) LDrr(registers.a, registers.l);
    else if(instruction == 0x7E) LDrr(registers.a, registers.h, registers.l);
    else if(instruction == 0x40) LDrr(registers.b, registers.b);
    else if(instruction == 0x41) LDrr(registers.b, registers.c);
    else if(instruction == 0x42) LDrr(registers.b, registers.d);
    else if(instruction == 0x43) LDrr(registers.b, registers.e);
    else if(instruction == 0x44) LDrr(registers.b, registers.h);
    else if(instruction == 0x45) LDrr(registers.b, registers.l);
    else if(instruction == 0x46) LDrr(registers.b, registers.h, registers.l);
}

void CPU::NOP()
{

}

void CPU::LDrr(uint8_t& r1, uint8_t& r2)
{
    r1 = r2;
}

void CPU::LDrr(uint8_t &r1, uint8_t &r2msb, uint8_t &r2lsb)
{

}




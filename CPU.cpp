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
        uint8_t immediate = memory->Read(registers.pc++);
        LDHLr(immediate);
    }
    else if(instruction == 0x7F) LDAn(registers.a);
    else if(instruction == 0x78) LDAn(registers.b);
    else if(instruction == 0x79) LDAn(registers.c);
    else if(instruction == 0x7A) LDAn(registers.d);
    else if(instruction == 0x7B) LDAn(registers.e);
    else if(instruction == 0x7C) LDAn(registers.h);
    else if(instruction == 0x7D) LDAn(registers.l);
    else if(instruction == 0x0A)
    {
        uint16_t address = BC();
        uint8_t data = memory->Read(address);
        LDAn(data);
    }
    else if(instruction == 0x1A)
    {
        uint16_t address = DE();
        uint8_t data = memory->Read(address);
        LDAn(data);
    }
    else if(instruction == 0x1A)
    {
        uint16_t address = HL();
        uint8_t data = memory->Read(address);
        LDAn(data);
    }
    else if(instruction == 0xFA)
    {
        uint8_t lsb = memory->Read(registers.pc++);
        uint8_t msb = memory->Read(registers.pc++);
        uint16_t addr = (uint16_t)(lsb << 8) | msb;
        LDAn(addr);
    }
    else if(instruction == 0x3E)
    {
        LDAn(registers.pc++);
    }
    else if(instruction == 0x47) LDnA(registers.b);
    else if(instruction == 0x4F) LDnA(registers.c);
    else if(instruction == 0x57) LDnA(registers.d);
    else if(instruction == 0x5F) LDnA(registers.e);
    else if(instruction == 0x67) LDnA(registers.h);
    else if(instruction == 0x6F) LDnA(registers.l);
    else if(instruction == 0x02) LDnA(BC());
    else if(instruction == 0x12) LDnA(DE());
    else if(instruction == 0x77) LDnA(HL());
    else if(instruction == 0xEA)
    {
        uint8_t lsb = memory->Read(registers.pc++);
        uint8_t msb = memory->Read(registers.pc++);
        uint16_t addr = (uint16_t)(lsb << 8) | msb;
        LDnA(addr);
    }
    else if(instruction == 0xF2) LDAC();
    else if(instruction == 0xE2) LDCA();
    else if(instruction == 0x3A) LDAHLdec();
    else if(instruction == 0x32) LDHLAdec();
    else if(instruction == 0x2A) LDAHLinc();
    else if(instruction == 0x22) LDHLAinc();
    else if(instruction == 0xE0) LDHnA(registers.pc++);
    else if(instruction == 0xF0) LDHAn(registers.pc++);
    else if(instruction == 0x01)
    {
        uint8_t lsb = registers.pc++;
        uint8_t msb = registers.pc++;
        uint16_t immediate = (uint16_t)(lsb << 8) | msb;
        LDBCnn(immediate);
    }
    else if(instruction == 0x11)
    {
        uint8_t lsb = registers.pc++;
        uint8_t msb = registers.pc++;
        uint16_t immediate = (uint16_t)(lsb << 8) | msb;
        LDDEnn(immediate);
    }
    else if(instruction == 0x21)
    {
        uint8_t lsb = registers.pc++;
        uint8_t msb = registers.pc++;
        uint16_t immediate = (uint16_t)(lsb << 8) | msb;
        LDHLnn(immediate);
    }
    else if(instruction == 0x31)
    {
        uint8_t lsb = registers.pc++;
        uint8_t msb = registers.pc++;
        uint16_t immediate = (uint16_t)(lsb << 8) | msb;
        LDSPnn(immediate);
    }
    else if(instruction == 0xF9) LDSPHL();


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
    uint16_t hl = HL();
    memory->Write(hl, r);
}

void CPU::LDrHL(uint8_t &r)
{
    uint16_t hl = HL();
    uint8_t data = memory->Read(hl);
    r = data;
}

void CPU::LDAn(uint8_t &r1)
{
    registers.a = r1;
}

void CPU::LDAn(uint16_t addr)
{
    registers.a = memory->Read(addr);
}

void CPU::LDnA(uint8_t &r)
{
    r = registers.a;
}

void CPU::LDnA(uint16_t addr)
{
    memory->Write(addr, registers.a);
}

void CPU::LDAC()
{
    uint16_t addr = 0xFF00 + registers.c;
    registers.a = memory->Read(addr);
}

void CPU::LDCA()
{
    uint16_t addr = 0xFF00 + registers.c;
    memory->Write(addr, registers.a);
}

void CPU::LDAHLdec()
{
    uint16_t hl = HL();
    registers.a = memory->Read(hl);
    hl--;
    SetHL(hl);
}

void CPU::LDAHLinc()
{
    uint16_t hl = HL();
    registers.a = memory->Read(hl);
    hl++;
    SetHL(hl);
}

void CPU::LDHLAdec()
{
    uint16_t hl = HL();
    memory->Write(hl, registers.a);
    hl--;
    SetHL(hl);
}

void CPU::SetBC(uint16_t bc)
{
    registers.b = bc >> 8;
    registers.c = bc & 0x00FF;
}

void CPU::SetDE(uint16_t de)
{
    registers.d = de >> 8;
    registers.e = de & 0x00FF;
}

void CPU::SetSP(uint16_t sp)
{
    registers.sp = sp;
}

void CPU::SetHL(uint16_t hl)
{
    registers.h = hl >> 8;
    registers.l = hl & 0x00FF;
}

void CPU::LDHLAinc()
{
    uint16_t hl = HL();
    memory->Write(hl, registers.a);
    hl++;
    SetHL(hl);
}

void CPU::LDHnA(uint8_t n)
{
    uint16_t addr = 0xFF00;
    addr += n;
    memory->Write(addr, registers.a);
}

void CPU::LDHAn(uint8_t n)
{
    uint16_t addr = 0xFF00;
    addr += n;
    registers.a = memory->Read(addr);
}

void CPU::LDBCnn(uint16_t i)
{
    SetBC(i);
}

void CPU::LDDEnn(uint16_t i)
{
    SetDE(i);
}

void CPU::LDHLnn(uint16_t i)
{
    SetHL(i);
}

void CPU::LDSPnn(uint16_t i)
{
    SetSP(i);
}

void CPU::LDSPHL()
{
    registers.sp = HL();
}











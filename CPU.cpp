//
// Created by lorenzo on 8/16/20.
//

#include "CPU.hpp"

CPU::CPU(Memory* m)
: memory(m)
{
    Reset();
    PopulateDispatchTable();
}

CPU::~CPU()
= default;

void CPU::PopulateDispatchTable()
{
    dispatchTable.insert(std::make_pair(0x00, std::bind(&CPU::NOP, this)));
    dispatchTable.insert(std::make_pair(0x04, std::bind(&CPU::INC_B, this)));
    dispatchTable.insert(std::make_pair(0x0C, std::bind(&CPU::INC_C, this)));
    dispatchTable.insert(std::make_pair(0x14, std::bind(&CPU::INC_D, this)));
    dispatchTable.insert(std::make_pair(0x1C, std::bind(&CPU::INC_E, this)));
    dispatchTable.insert(std::make_pair(0x24, std::bind(&CPU::INC_H, this)));
    dispatchTable.insert(std::make_pair(0x2C, std::bind(&CPU::INC_L, this)));
    dispatchTable.insert(std::make_pair(0x34, std::bind(&CPU::INC_HL, this)));
    dispatchTable.insert(std::make_pair(0x3C, std::bind(&CPU::INC_A, this)));
    dispatchTable.insert(std::make_pair(0x80, std::bind(&CPU::ADD_B, this)));
    dispatchTable.insert(std::make_pair(0x81, std::bind(&CPU::ADD_C, this)));
    dispatchTable.insert(std::make_pair(0x82, std::bind(&CPU::ADD_D, this)));
    dispatchTable.insert(std::make_pair(0x83, std::bind(&CPU::ADD_E, this)));
    dispatchTable.insert(std::make_pair(0x84, std::bind(&CPU::ADD_H, this)));
    dispatchTable.insert(std::make_pair(0x85, std::bind(&CPU::ADD_L, this)));
    dispatchTable.insert(std::make_pair(0x86, std::bind(&CPU::ADD_HL, this)));
    dispatchTable.insert(std::make_pair(0x87, std::bind(&CPU::ADD_A, this)));
    dispatchTable.insert(std::make_pair(0x88, std::bind(&CPU::ADC_B, this)));
    dispatchTable.insert(std::make_pair(0x89, std::bind(&CPU::ADC_C, this)));
    dispatchTable.insert(std::make_pair(0x8A, std::bind(&CPU::ADC_D, this)));
    dispatchTable.insert(std::make_pair(0x8B, std::bind(&CPU::ADC_E, this)));
    dispatchTable.insert(std::make_pair(0x8C, std::bind(&CPU::ADC_H, this)));
    dispatchTable.insert(std::make_pair(0x8D, std::bind(&CPU::ADC_L, this)));
    dispatchTable.insert(std::make_pair(0x8E, std::bind(&CPU::ADC_HL, this)));
    dispatchTable.insert(std::make_pair(0x8F, std::bind(&CPU::ADC_A, this)));
    dispatchTable.insert(std::make_pair(0x90, std::bind(&CPU::SUB_B, this)));
    dispatchTable.insert(std::make_pair(0x91, std::bind(&CPU::SUB_C, this)));
    dispatchTable.insert(std::make_pair(0x92, std::bind(&CPU::SUB_D, this)));
    dispatchTable.insert(std::make_pair(0x93, std::bind(&CPU::SUB_E, this)));
    dispatchTable.insert(std::make_pair(0x94, std::bind(&CPU::SUB_H, this)));
    dispatchTable.insert(std::make_pair(0x95, std::bind(&CPU::SUB_L, this)));
    dispatchTable.insert(std::make_pair(0x96, std::bind(&CPU::SUB_HL, this)));
    dispatchTable.insert(std::make_pair(0x97, std::bind(&CPU::SUB_A, this)));
    dispatchTable.insert(std::make_pair(0x98, std::bind(&CPU::SBC_B, this)));
    dispatchTable.insert(std::make_pair(0x99, std::bind(&CPU::SBC_C, this)));
    dispatchTable.insert(std::make_pair(0x9A, std::bind(&CPU::SBC_D, this)));
    dispatchTable.insert(std::make_pair(0x9B, std::bind(&CPU::SBC_E, this)));
    dispatchTable.insert(std::make_pair(0x9C, std::bind(&CPU::SBC_H, this)));
    dispatchTable.insert(std::make_pair(0x9D, std::bind(&CPU::SBC_L, this)));
    dispatchTable.insert(std::make_pair(0x9E, std::bind(&CPU::SBC_HL, this)));
    dispatchTable.insert(std::make_pair(0x9F, std::bind(&CPU::SBC_A, this)));
    dispatchTable.insert(std::make_pair(0xA0, std::bind(&CPU::AND_B, this)));
    dispatchTable.insert(std::make_pair(0xA1, std::bind(&CPU::AND_C, this)));
    dispatchTable.insert(std::make_pair(0xA2, std::bind(&CPU::AND_D, this)));
    dispatchTable.insert(std::make_pair(0xA3, std::bind(&CPU::AND_E, this)));
    dispatchTable.insert(std::make_pair(0xA4, std::bind(&CPU::AND_H, this)));
    dispatchTable.insert(std::make_pair(0xA5, std::bind(&CPU::AND_L, this)));
    dispatchTable.insert(std::make_pair(0xA6, std::bind(&CPU::AND_HL, this)));
    dispatchTable.insert(std::make_pair(0xA7, std::bind(&CPU::AND_A, this)));
    dispatchTable.insert(std::make_pair(0xA8, std::bind(&CPU::XOR_B, this)));
    dispatchTable.insert(std::make_pair(0xA9, std::bind(&CPU::XOR_C, this)));
    dispatchTable.insert(std::make_pair(0xAA, std::bind(&CPU::XOR_D, this)));
    dispatchTable.insert(std::make_pair(0xAB, std::bind(&CPU::XOR_E, this)));
    dispatchTable.insert(std::make_pair(0xAC, std::bind(&CPU::XOR_H, this)));
    dispatchTable.insert(std::make_pair(0xAD, std::bind(&CPU::XOR_L, this)));
    dispatchTable.insert(std::make_pair(0xAE, std::bind(&CPU::XOR_HL, this)));
    dispatchTable.insert(std::make_pair(0xAF, std::bind(&CPU::XOR_A, this)));
    dispatchTable.insert(std::make_pair(0xB0, std::bind(&CPU::OR_B, this)));
    dispatchTable.insert(std::make_pair(0xB1, std::bind(&CPU::OR_C, this)));
    dispatchTable.insert(std::make_pair(0xB2, std::bind(&CPU::OR_D, this)));
    dispatchTable.insert(std::make_pair(0xB3, std::bind(&CPU::OR_E, this)));
    dispatchTable.insert(std::make_pair(0xB4, std::bind(&CPU::OR_H, this)));
    dispatchTable.insert(std::make_pair(0xB5, std::bind(&CPU::OR_L, this)));
    dispatchTable.insert(std::make_pair(0xB6, std::bind(&CPU::OR_HL, this)));
    dispatchTable.insert(std::make_pair(0xB7, std::bind(&CPU::OR_A, this)));
    dispatchTable.insert(std::make_pair(0xB8, std::bind(&CPU::CP_B, this)));
    dispatchTable.insert(std::make_pair(0xB9, std::bind(&CPU::CP_C, this)));
    dispatchTable.insert(std::make_pair(0xBA, std::bind(&CPU::CP_D, this)));
    dispatchTable.insert(std::make_pair(0xBB, std::bind(&CPU::CP_E, this)));
    dispatchTable.insert(std::make_pair(0xBC, std::bind(&CPU::CP_H, this)));
    dispatchTable.insert(std::make_pair(0xBD, std::bind(&CPU::CP_L, this)));
    dispatchTable.insert(std::make_pair(0xBE, std::bind(&CPU::CP_HL, this)));
    dispatchTable.insert(std::make_pair(0xBF, std::bind(&CPU::CP_A, this)));
    dispatchTable.insert(std::make_pair(0xC1, std::bind(&CPU::POP_BC, this)));
    dispatchTable.insert(std::make_pair(0xC5, std::bind(&CPU::PUSH_BC, this)));
    dispatchTable.insert(std::make_pair(0xC6, std::bind(&CPU::ADD_Immediate, this)));
    dispatchTable.insert(std::make_pair(0xCE, std::bind(&CPU::ADC_Immediate, this)));
    dispatchTable.insert(std::make_pair(0xD1, std::bind(&CPU::POP_DE, this)));
    dispatchTable.insert(std::make_pair(0xD5, std::bind(&CPU::PUSH_DE, this)));
    dispatchTable.insert(std::make_pair(0xD6, std::bind(&CPU::SUB_Immediate, this)));
    dispatchTable.insert(std::make_pair(0xDE, std::bind(&CPU::SBC_Immediate, this)));
    dispatchTable.insert(std::make_pair(0xE1, std::bind(&CPU::POP_HL, this)));
    dispatchTable.insert(std::make_pair(0xE5, std::bind(&CPU::PUSH_HL, this)));
    dispatchTable.insert(std::make_pair(0xE6, std::bind(&CPU::AND_Immediate, this)));
    dispatchTable.insert(std::make_pair(0xEE, std::bind(&CPU::XOR_Immediate, this)));
    dispatchTable.insert(std::make_pair(0xF1, std::bind(&CPU::POP_AF, this)));
    dispatchTable.insert(std::make_pair(0xF5, std::bind(&CPU::PUSH_AF, this)));
    dispatchTable.insert(std::make_pair(0xF6, std::bind(&CPU::OR_Immediate, this)));
    dispatchTable.insert(std::make_pair(0xFE, std::bind(&CPU::CP_Immediate, this)));
}

void CPU::SetZeroFlag()
{
    registers.f |= 0b10000000;
}

void CPU::ClearZeroFlag()
{
    registers.f &= ~0b10000000;
}

uint8_t CPU::GetCarryFlag()
{
    return registers.f & 0b10000000;
}

void CPU::SetSubtractFlag()
{
    registers.f |= 0b01000000;
}

void CPU::ClearSubtractFlag()
{
    registers.f &= ~0b01000000;
}

void CPU::SetHalfCarryFlag()
{
    registers.f |= 0b00100000;
}

void CPU::ClearHalfCarryFlag()
{
    registers.f &= ~0b00100000;
}

void CPU::SetCarryFlag()
{
    registers.f |= 0b00010000;
}

void CPU::ClearCarryFlag()
{
    registers.f &= ~0b00010000;
}

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
    if (dispatchTable.find(instruction) == dispatchTable.end()) 
    {

    }
    else 
    {
        dispatchTable[instruction]();
    }
/*
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
    else if(instruction == 0x7E) LDrHL(registers.a);
    else if(instruction == 0x40) LDrr(registers.b, registers.b);
    else if(instruction == 0x41) LDrr(registers.b, registers.c);
    else if(instruction == 0x42) LDrr(registers.b, registers.d);
    else if(instruction == 0x43) LDrr(registers.b, registers.e);
    else if(instruction == 0x44) LDrr(registers.b, registers.h);
    else if(instruction == 0x45) LDrr(registers.b, registers.l);
    else if(instruction == 0x46) LDrHL(registers.b);
    else if(instruction == 0x48) LDrr(registers.c, registers.b);
    else if(instruction == 0x49) LDrr(registers.c, registers.c);
    else if(instruction == 0x4A) LDrr(registers.c, registers.d);
    else if(instruction == 0x4B) LDrr(registers.c, registers.e);
    else if(instruction == 0x4C) LDrr(registers.c, registers.h);
    else if(instruction == 0x4D) LDrr(registers.c, registers.l);
    else if(instruction == 0x4E) LDrHL(registers.c);
    else if(instruction == 0x50) LDrr(registers.d, registers.b);
    else if(instruction == 0x51) LDrr(registers.d, registers.c);
    else if(instruction == 0x52) LDrr(registers.d, registers.d);
    else if(instruction == 0x53) LDrr(registers.d, registers.e);
    else if(instruction == 0x54) LDrr(registers.d, registers.h);
    else if(instruction == 0x55) LDrr(registers.d, registers.l);
    else if(instruction == 0x56) LDrHL(registers.d);
    else if(instruction == 0x58) LDrr(registers.e, registers.b);
    else if(instruction == 0x59) LDrr(registers.e, registers.c);
    else if(instruction == 0x5A) LDrr(registers.e, registers.d);
    else if(instruction == 0x5B) LDrr(registers.e, registers.e);
    else if(instruction == 0x5C) LDrr(registers.e, registers.h);
    else if(instruction == 0x5D) LDrr(registers.e, registers.l);
    else if(instruction == 0x5E) LDrHL(registers.e);
    else if(instruction == 0x60) LDrr(registers.h, registers.b);
    else if(instruction == 0x61) LDrr(registers.h, registers.c);
    else if(instruction == 0x62) LDrr(registers.h, registers.d);
    else if(instruction == 0x63) LDrr(registers.h, registers.e);
    else if(instruction == 0x64) LDrr(registers.h, registers.h);
    else if(instruction == 0x65) LDrr(registers.h, registers.l);
    else if(instruction == 0x66) LDrHL(registers.h);
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
    else if(instruction == 0xF8) LDHLSPn(registers.pc++);
    else if(instruction == 0x08)
    {
        uint8_t lsb = registers.pc++;
        uint8_t msb = registers.pc++;
        uint16_t address = (uint16_t)(lsb << 8) | msb;
        LDnnSP(address);
    }
    */
}

void CPU::NOP()
{
    DEBUG("0x00 NOP");
    DEBUG(" ");
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

void CPU::LDHLSPn(uint8_t n)
{
    uint16_t addr = registers.sp + n;
    SetSP(addr);
}

void CPU::LDnnSP(uint16_t nn)
{
    registers.sp = nn;
}

void CPU::PUSH_AF()
{
    PUSH(AF());
}

void CPU::PUSH_BC()
{
    PUSH(BC());
}

void CPU::PUSH_DE()
{
    PUSH(DE());
}

void CPU::PUSH_HL()
{
    PUSH(HL());
}

void CPU::PUSH(uint16_t op)
{
    registers.sp--;
    uint8_t msb = (uint8_t) op >> 8;
    uint8_t lsb = (uint8_t) op;

    memory->Write(registers.sp--, msb);
    memory->Write(registers.sp--, lsb);
}

void CPU::POP_AF()
{
    registers.a = memory->Read(registers.sp++);
    registers.f = memory->Read(registers.sp++);
}

void CPU::POP_BC()
{
    registers.b = memory->Read(registers.sp++);
    registers.c = memory->Read(registers.sp++);
}

void CPU::POP_DE()
{
    registers.d = memory->Read(registers.sp++);
    registers.e = memory->Read(registers.sp++);
}

void CPU::POP_HL()
{
    registers.h = memory->Read(registers.sp++);
    registers.l = memory->Read(registers.sp++);
}

void CPU::ADD_A()
{
    DEBUG("0x87 ADD A,A");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);

    ADD(registers.a);
}

void CPU::ADD_B()
{
    DEBUG("0x80 ADD A,B");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register B");
    DEBUG_PRINT_REGISTER(registers.b);

    ADD(registers.b);
}

void CPU::ADD_C()
{
    DEBUG("0x81 ADD A,C");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register C");
    DEBUG_PRINT_REGISTER(registers.c);

    ADD(registers.c);
}

void CPU::ADD_D()
{
    DEBUG("0x82 ADD A,D");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register D");
    DEBUG_PRINT_REGISTER(registers.d);

    ADD(registers.d);
}

void CPU::ADD_E()
{
    DEBUG("0x83 ADD A,E");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register E");
    DEBUG_PRINT_REGISTER(registers.e);

    ADD(registers.e);
}

void CPU::ADD_H()
{
    DEBUG("0x84 ADD A,H");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register H");
    DEBUG_PRINT_REGISTER(registers.h);

    ADD(registers.h);
}

void CPU::ADD_L()
{
    DEBUG("0x85 ADD A,L");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register L");
    DEBUG_PRINT_REGISTER(registers.l);

    ADD(registers.l);
}

void CPU::ADD_HL()
{
    DEBUG("0x86 ADD A,(HL)");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register H");
    DEBUG_PRINT_REGISTER(registers.h);
    DEBUG("Register L");
    DEBUG_PRINT_REGISTER(registers.l);
    DEBUG("Register HL");
    DEBUG_PRINT_REGISTER(HL());
    DEBUG("Value at (HL)");
    DEBUG_PRINT_REGISTER(memory->Read(HL()));

    ADD(memory->Read(HL()));
}

void CPU::ADD_Immediate()
{
    DEBUG("0xC6 ADD A,#");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);

    ADD(registers.pc++);
}

void CPU::ADD(uint8_t op)
{
    DEBUG("OPERAND");
    DEBUG_PRINT_REGISTER(op);
    uint16_t result = registers.a + op;
    if(result == 0x00) SetZeroFlag();
    if(result > 0xFF) SetCarryFlag();
    if((registers.a & 0xF) + (op & 0xF) > 0xF) SetHalfCarryFlag();
    ClearSubtractFlag();
    registers.a = (uint8_t)result;
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG(" ");
}

void CPU::ADC_A()
{
    DEBUG("0x8F ADC A,A");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Carry");

    uint8_t carryFlag = GetCarryFlag();
    DEBUG_PRINT_REGISTER(carryFlag);
    uint16_t result = registers.a + registers.a + carryFlag;
    if(result == 0x00) SetZeroFlag();
    if(result > 0xFF) SetCarryFlag();
    if((registers.a & 0xF) + ((registers.a + carryFlag) & 0xF) > 0xF) SetHalfCarryFlag();
    ClearSubtractFlag();
    registers.a = (uint8_t)result;

    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG(" ");
}

void CPU::ADC_B()
{
    DEBUG("0x88 ADC A,B");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register B");
    DEBUG_PRINT_REGISTER(registers.b);
    DEBUG("Carry");

    uint8_t carryFlag = GetCarryFlag();
    DEBUG_PRINT_REGISTER(carryFlag);
    uint16_t result = registers.a + registers.b + carryFlag;
    if(result == 0x00) SetZeroFlag();
    if(result > 0xFF) SetCarryFlag();
    if((registers.a & 0xF) + ((registers.b + carryFlag) & 0xF) > 0xF) SetHalfCarryFlag();
    ClearSubtractFlag();
    registers.a = (uint8_t)result;

    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG(" ");
}

void CPU::ADC_C()
{
    DEBUG("0x89 ADC A,C");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register C");
    DEBUG_PRINT_REGISTER(registers.c);
    DEBUG("Carry");

    uint8_t carryFlag = GetCarryFlag();
    DEBUG_PRINT_REGISTER(carryFlag);
    uint16_t result = registers.a + registers.c + carryFlag;
    if(result == 0x00) SetZeroFlag();
    if(result > 0xFF) SetCarryFlag();
    if((registers.a & 0xF) + ((registers.c + carryFlag) & 0xF) > 0xF) SetHalfCarryFlag();
    ClearSubtractFlag();
    registers.a = (uint8_t)result;

    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG(" ");
}

void CPU::ADC_D()
{
    DEBUG("0x8A ADC A,D");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register D");
    DEBUG_PRINT_REGISTER(registers.d);
    DEBUG("Carry");

    uint8_t carryFlag = GetCarryFlag();
    DEBUG_PRINT_REGISTER(carryFlag);
    uint16_t result = registers.a + registers.d + carryFlag;
    if(result == 0x00) SetZeroFlag();
    if(result > 0xFF) SetCarryFlag();
    if((registers.a & 0xF) + ((registers.d + carryFlag) & 0xF) > 0xF) SetHalfCarryFlag();
    ClearSubtractFlag();
    registers.a = (uint8_t)result;

    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG(" ");
}

void CPU::ADC_E()
{
    DEBUG("0x8B ADC A,E");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register E");
    DEBUG_PRINT_REGISTER(registers.e);
    DEBUG("Carry");

    uint8_t carryFlag = GetCarryFlag();
    DEBUG_PRINT_REGISTER(carryFlag);
    uint16_t result = registers.a + registers.e + carryFlag;
    if(result == 0x00) SetZeroFlag();
    if(result > 0xFF) SetCarryFlag();
    if((registers.a & 0xF) + ((registers.e + carryFlag) & 0xF) > 0xF) SetHalfCarryFlag();
    ClearSubtractFlag();
    registers.a = (uint8_t)result;

    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG(" ");
}

void CPU::ADC_H()
{
    DEBUG("0x8C ADC A,H");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register H");
    DEBUG_PRINT_REGISTER(registers.h);
    DEBUG("Carry");

    uint8_t carryFlag = GetCarryFlag();
    DEBUG_PRINT_REGISTER(carryFlag);
    uint16_t result = registers.a + registers.h + carryFlag;
    if(result == 0x00) SetZeroFlag();
    if(result > 0xFF) SetCarryFlag();
    if((registers.a & 0xF) + ((registers.h + carryFlag) & 0xF) > 0xF) SetHalfCarryFlag();
    ClearSubtractFlag();
    registers.a = (uint8_t)result;

    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG(" ");
}

void CPU::ADC_L()
{
    DEBUG("0x8D ADC A,L");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register L");
    DEBUG_PRINT_REGISTER(registers.l);
    DEBUG("Carry");

    uint8_t carryFlag = GetCarryFlag();
    DEBUG_PRINT_REGISTER(carryFlag);
    uint16_t result = registers.a + registers.l + carryFlag;
    if(result == 0x00) SetZeroFlag();
    if(result > 0xFF) SetCarryFlag();
    if((registers.a & 0xF) + ((registers.l + carryFlag) & 0xF) > 0xF) SetHalfCarryFlag();
    ClearSubtractFlag();
    registers.a = (uint8_t)result;

    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG(" ");
}

void CPU::ADC_HL()
{
    DEBUG("0x8E ADC A,(HL)");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register H");
    DEBUG_PRINT_REGISTER(registers.h);
    DEBUG("Register L");
    DEBUG_PRINT_REGISTER(registers.l);
    DEBUG("Register HL");
    DEBUG_PRINT_REGISTER(HL());
    DEBUG("Value at (HL)");
    DEBUG_PRINT_REGISTER(memory->Read(HL()));
    DEBUG("Carry");

    uint8_t carryFlag = GetCarryFlag();
    DEBUG_PRINT_REGISTER(carryFlag);
    uint8_t val = memory->Read(HL());
    uint16_t result = registers.a + val + carryFlag;
    if(result == 0x00) SetZeroFlag();
    if(result > 0xFF) SetCarryFlag();
    if((registers.a & 0xF) + ((val + carryFlag) & 0xF) > 0xF) SetHalfCarryFlag();
    ClearSubtractFlag();
    registers.a = (uint8_t)result;

    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG(" ");
}

void CPU::ADC_Immediate()
{
    DEBUG("0xCE ADC A,#");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Carry");
    uint8_t carryFlag = GetCarryFlag();
    DEBUG_PRINT_REGISTER(carryFlag);

    uint8_t val = registers.pc++;
    uint16_t result = registers.a + val + carryFlag;
    if(result == 0x00) SetZeroFlag();
    if(result > 0xFF) SetCarryFlag();
    if((registers.a & 0xF) + (val + carryFlag & 0xF) > 0xF) SetHalfCarryFlag();
    ClearSubtractFlag();
    registers.a = (uint8_t)result;

    DEBUG("OPERAND");
    DEBUG_PRINT_REGISTER(registers.pc);
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG(" ");
}

void CPU::SUB_A()
{
    DEBUG("0x97 SUB A,A");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);

    uint16_t result = registers.a - registers.a;

    if(result == 0x0) SetZeroFlag();
    if(result < 0x0) SetCarryFlag();
    if(((registers.a & 0xF) - (registers.a & 0xF)) < 0x0) SetHalfCarryFlag();
    SetSubtractFlag();

    registers.a = (uint8_t)result;

    DEBUG("OPERAND");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG(" ");
}

void CPU::SUB_B()
{
    DEBUG("0x90 SUB A,B");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register B");
    DEBUG_PRINT_REGISTER(registers.b);

    uint16_t result = registers.a - registers.b;
    
    if(result == 0x0) SetZeroFlag();
    if(result < 0x0) SetCarryFlag();
    if(((registers.a & 0xF) - (registers.b & 0xF)) < 0x0) SetHalfCarryFlag();
    SetSubtractFlag();

    registers.a = (uint8_t)result;

    DEBUG("OPERAND");
    DEBUG_PRINT_REGISTER(registers.b);
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG(" ");
}

void CPU::SUB_C()
{
    DEBUG("0x91 SUB A,C");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register C");
    DEBUG_PRINT_REGISTER(registers.c);

    uint16_t result = registers.a - registers.c;
    
    if(result == 0x0) SetZeroFlag();
    if(result < 0x0) SetCarryFlag();
    if(((registers.a & 0xF) - (registers.c & 0xF)) < 0x0) SetHalfCarryFlag();
    SetSubtractFlag();

    registers.a = (uint8_t)result;

    DEBUG("OPERAND");
    DEBUG_PRINT_REGISTER(registers.c);
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG(" ");
}

void CPU::SUB_D()
{
    DEBUG("0x92 SUB A,D");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register D");
    DEBUG_PRINT_REGISTER(registers.d);

    uint16_t result = registers.a - registers.d;
    
    if(result == 0x0) SetZeroFlag();
    if(result < 0x0) SetCarryFlag();
    if(((registers.a & 0xF) - (registers.d & 0xF)) < 0x0) SetHalfCarryFlag();
    SetSubtractFlag();

    registers.a = (uint8_t)result;

    DEBUG("OPERAND");
    DEBUG_PRINT_REGISTER(registers.d);
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG(" ");
}

void CPU::SUB_E()
{
    DEBUG("0x93 SUB A,E");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register E");
    DEBUG_PRINT_REGISTER(registers.e);

    uint16_t result = registers.a - registers.e;
    
    if(result == 0x0) SetZeroFlag();
    if(result < 0x0) SetCarryFlag();
    if(((registers.a & 0xF) - (registers.e & 0xF)) < 0x0) SetHalfCarryFlag();
    SetSubtractFlag();

    registers.a = (uint8_t)result;

    DEBUG("OPERAND");
    DEBUG_PRINT_REGISTER(registers.e);
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG(" ");
}

void CPU::SUB_H()
{
    DEBUG("0x94 SUB A,H");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register H");
    DEBUG_PRINT_REGISTER(registers.h);

    uint16_t result = registers.a - registers.h;
    
    if(result == 0x0) SetZeroFlag();
    if(result < 0x0) SetCarryFlag();
    if(((registers.a & 0xF) - (registers.h & 0xF)) < 0x0) SetHalfCarryFlag();
    SetSubtractFlag();

    registers.a = (uint8_t)result;

    DEBUG("OPERAND");
    DEBUG_PRINT_REGISTER(registers.h);
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG(" ");
}

void CPU::SUB_L()
{
    DEBUG("0x94 SUB A,L");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register L");
    DEBUG_PRINT_REGISTER(registers.l);

    uint16_t result = registers.a - registers.l;
    
    if(result == 0x0) SetZeroFlag();
    if(result < 0x0) SetCarryFlag();
    if(((registers.a & 0xF) - (registers.l & 0xF)) < 0x0) SetHalfCarryFlag();
    SetSubtractFlag();

    registers.a = (uint8_t)result;

    DEBUG("OPERAND");
    DEBUG_PRINT_REGISTER(registers.l);
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG(" ");
}

void CPU::SUB_HL()
{
    DEBUG("0x96 SUB A,(HL)");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register H");
    DEBUG_PRINT_REGISTER(registers.h);
    DEBUG("Register L");
    DEBUG_PRINT_REGISTER(registers.l);
    DEBUG("Register HL");
    DEBUG_PRINT_REGISTER(HL());
    DEBUG("Value at (HL)");
    DEBUG_PRINT_REGISTER(memory->Read(HL()));

    uint8_t val = memory->Read(HL());

    uint16_t result = registers.a - val;
    
    if(result == 0x0) SetZeroFlag();
    if(result < 0x0) SetCarryFlag();
    if(((registers.a & 0xF) - (val & 0xF)) < 0x0) SetHalfCarryFlag();
    SetSubtractFlag();

    registers.a = (uint8_t)result;

    DEBUG("OPERAND");
    DEBUG_PRINT_REGISTER(val);
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG(" ");

}

void CPU::SUB_Immediate()
{
    DEBUG("0xD6 SUB A,#");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);

    uint8_t val = registers.pc++;
    uint16_t result = registers.a - val;
    
    if(result == 0x0) SetZeroFlag();
    if(result < 0x0) SetCarryFlag();
    if(((registers.a & 0xF) - (val & 0xF)) < 0x0) SetHalfCarryFlag();
    SetSubtractFlag();

    registers.a = (uint8_t)result;

    DEBUG("OPERAND");
    DEBUG_PRINT_REGISTER(val);
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG(" ");
}

void CPU::SBC_A()
{
    DEBUG("0x9F SBC A,A");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("CARRY");
    auto carry = GetCarryFlag();
    DEBUG_PRINT_REGISTER(carry);

    uint8_t val = registers.a;
    uint16_t result = registers.a - (val + carry);

    if(result == 0x0) SetZeroFlag();
    if(result < 0x0) SetCarryFlag();
    if((registers.a & 0xF) - (val + carry & 0xF) < 0xF) SetHalfCarryFlag();
    SetSubtractFlag();

    registers.a = (uint8_t)result;

    DEBUG("OPERAND");
    DEBUG_PRINT_REGISTER(val + carry);
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG(" ");
}

void CPU::SBC_B()
{
    DEBUG("0x98 SBC A,B");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register B");
    DEBUG_PRINT_REGISTER(registers.b);
    DEBUG("CARRY");
    auto carry = GetCarryFlag();
    DEBUG_PRINT_REGISTER(carry);

    uint8_t val = registers.b;
    uint16_t result = registers.a - (val + carry);

    if(result == 0x0) SetZeroFlag();
    if(result < 0x0) SetCarryFlag();
    if((registers.a & 0xF) - (val + carry & 0xF) < 0xF) SetHalfCarryFlag();
    SetSubtractFlag();

    registers.a = (uint8_t)result;

    DEBUG("OPERAND");
    DEBUG_PRINT_REGISTER(val + carry);
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG(" ");
}

void CPU::SBC_C()
{
    DEBUG("0x99 SBC A,C");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register C");
    DEBUG_PRINT_REGISTER(registers.c);
    DEBUG("CARRY");
    auto carry = GetCarryFlag();
    DEBUG_PRINT_REGISTER(carry);

    uint8_t val = registers.c;
    uint16_t result = registers.a - (val + carry);

    if(result == 0x0) SetZeroFlag();
    if(result < 0x0) SetCarryFlag();
    if((registers.a & 0xF) - (val + carry & 0xF) < 0xF) SetHalfCarryFlag();
    SetSubtractFlag();

    registers.a = (uint8_t)result;

    DEBUG("OPERAND");
    DEBUG_PRINT_REGISTER(val + carry);
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG(" ");
}

void CPU::SBC_D()
{
    DEBUG("0x9A SBC A,D");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register D");
    DEBUG_PRINT_REGISTER(registers.d);
    DEBUG("CARRY");
    auto carry = GetCarryFlag();
    DEBUG_PRINT_REGISTER(carry);

    uint8_t val = registers.d;
    uint16_t result = registers.a - (val + carry);

    if(result == 0x0) SetZeroFlag();
    if(result < 0x0) SetCarryFlag();
    if((registers.a & 0xF) - (val + carry & 0xF) < 0xF) SetHalfCarryFlag();
    SetSubtractFlag();

    registers.a = (uint8_t)result;

    DEBUG("OPERAND");
    DEBUG_PRINT_REGISTER(val + carry);
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG(" ");
}

void CPU::SBC_E()
{
    DEBUG("0x9B SBC A,E");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register E");
    DEBUG_PRINT_REGISTER(registers.e);
    DEBUG("CARRY");
    auto carry = GetCarryFlag();
    DEBUG_PRINT_REGISTER(carry);

    uint8_t val = registers.e;
    uint16_t result = registers.a - (val + carry);

    if(result == 0x0) SetZeroFlag();
    if(result < 0x0) SetCarryFlag();
    if((registers.a & 0xF) - (val + carry & 0xF) < 0xF) SetHalfCarryFlag();
    SetSubtractFlag();

    registers.a = (uint8_t)result;

    DEBUG("OPERAND");
    DEBUG_PRINT_REGISTER(val + carry);
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG(" ");
}

void CPU::SBC_H()
{
    DEBUG("0x9C SBC A,H");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register H");
    DEBUG_PRINT_REGISTER(registers.h);
    DEBUG("CARRY");
    auto carry = GetCarryFlag();
    DEBUG_PRINT_REGISTER(carry);

    uint8_t val = registers.h;
    uint16_t result = registers.a - (val + carry);

    if(result == 0x0) SetZeroFlag();
    if(result < 0x0) SetCarryFlag();
    if((registers.a & 0xF) - (val + carry & 0xF) < 0xF) SetHalfCarryFlag();
    SetSubtractFlag();

    registers.a = (uint8_t)result;

    DEBUG("OPERAND");
    DEBUG_PRINT_REGISTER(val + carry);
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG(" ");
}

void CPU::SBC_L()
{
    DEBUG("0x9D SBC A,L");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register L");
    DEBUG_PRINT_REGISTER(registers.l);
    DEBUG("CARRY");
    auto carry = GetCarryFlag();
    DEBUG_PRINT_REGISTER(carry);

    uint8_t val = registers.l;
    uint16_t result = registers.a - (val + carry);

    if(result == 0x0) SetZeroFlag();
    if(result < 0x0) SetCarryFlag();
    if((registers.a & 0xF) - (val + carry & 0xF) < 0xF) SetHalfCarryFlag();
    SetSubtractFlag();

    registers.a = (uint8_t)result;

    DEBUG("OPERAND");
    DEBUG_PRINT_REGISTER(val + carry);
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG(" ");
}

void CPU::SBC_HL()
{
    DEBUG("0x9E SBC A,(HL)");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register H");
    DEBUG_PRINT_REGISTER(registers.h);
    DEBUG("Register L");
    DEBUG_PRINT_REGISTER(registers.l);
    DEBUG("Register HL");
    DEBUG_PRINT_REGISTER(HL());
    DEBUG("Value at (HL)");
    DEBUG_PRINT_REGISTER(memory->Read(HL()));
    DEBUG("CARRY");
    auto carry = GetCarryFlag();
    DEBUG_PRINT_REGISTER(carry);

    uint8_t val = memory->Read(HL());
    uint16_t result = registers.a - (val + carry);

    if(result == 0x0) SetZeroFlag();
    if(result < 0x0) SetCarryFlag();
    if((registers.a & 0xF) - (val + carry & 0xF) < 0xF) SetHalfCarryFlag();
    SetSubtractFlag();

    registers.a = (uint8_t)result;

    DEBUG("OPERAND");
    DEBUG_PRINT_REGISTER(val + carry);
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG(" ");
}

void CPU::SBC_Immediate()
{
    DEBUG("0xDE SBC A,#");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("CARRY");
    auto carry = GetCarryFlag();
    DEBUG_PRINT_REGISTER(carry);

    uint8_t val = registers.pc++;
    uint16_t result = registers.a - (val + carry);

    if(result == 0x0) SetZeroFlag();
    if(result < 0x0) SetCarryFlag();
    if((registers.a & 0xF) - (val + carry & 0xF) < 0xF) SetHalfCarryFlag();
    SetSubtractFlag();

    registers.a = (uint8_t)result;

    DEBUG("OPERAND");
    DEBUG_PRINT_REGISTER(val + carry);
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG(" ");
}

void CPU::AND_A()
{
    DEBUG("0xA7 AND A,A");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);

    uint8_t tReg = registers.a & registers.a;
    if(tReg == 0x00) SetZeroFlag();
    ClearSubtractFlag();
    SetHalfCarryFlag();
    ClearCarryFlag();
    registers.a = tReg;
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
}

void CPU::AND_B()
{
    DEBUG("0xA0 AND A,B");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register B");
    DEBUG_PRINT_REGISTER(registers.b);

    uint8_t tReg = registers.a & registers.b;
    if(tReg == 0x00) SetZeroFlag();
    ClearSubtractFlag();
    SetHalfCarryFlag();
    ClearCarryFlag();
    registers.a = tReg;
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
}

void CPU::AND_C()
{
    DEBUG("0xA1 AND A,C");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register C");
    DEBUG_PRINT_REGISTER(registers.c);

    uint8_t tReg = registers.a & registers.c;
    if(tReg == 0x00) SetZeroFlag();
    ClearSubtractFlag();
    SetHalfCarryFlag();
    ClearCarryFlag();
    registers.a = tReg;
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
}

void CPU::AND_D()
{
    DEBUG("0xA2 AND A,D");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register D");
    DEBUG_PRINT_REGISTER(registers.d);

    uint8_t tReg = registers.a & registers.d;
    if(tReg == 0x00) SetZeroFlag();
    ClearSubtractFlag();
    SetHalfCarryFlag();
    ClearCarryFlag();
    registers.a = tReg;
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
}

void CPU::AND_E()
{
    DEBUG("0xA3 AND A,E");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register E");
    DEBUG_PRINT_REGISTER(registers.e);

    uint8_t tReg = registers.a & registers.e;
    if(tReg == 0x00) SetZeroFlag();
    ClearSubtractFlag();
    SetHalfCarryFlag();
    ClearCarryFlag();
    registers.a = tReg;
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
}

void CPU::AND_H()
{
    DEBUG("0xA4 AND A,H");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register H");
    DEBUG_PRINT_REGISTER(registers.h);

    uint8_t tReg = registers.a & registers.h;
    if(tReg == 0x00) SetZeroFlag();
    ClearSubtractFlag();
    SetHalfCarryFlag();
    ClearCarryFlag();
    registers.a = tReg;
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
}

void CPU::AND_L()
{
    DEBUG("0xA5 AND A,L");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register L");
    DEBUG_PRINT_REGISTER(registers.l);

    uint8_t tReg = registers.a & registers.l;
    if(tReg == 0x00) SetZeroFlag();
    ClearSubtractFlag();
    SetHalfCarryFlag();
    ClearCarryFlag();
    registers.a = tReg;
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
}

void CPU::AND_HL()
{
    DEBUG("0xA6 AND A,HL");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);

    uint8_t val = memory->Read(HL());
    DEBUG("Value at (HL)");
    DEBUG_PRINT_REGISTER(val);

    uint8_t tReg = registers.a & val;
    if(tReg == 0x00) SetZeroFlag();
    ClearSubtractFlag();
    SetHalfCarryFlag();
    ClearCarryFlag();
    registers.a = tReg;
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
}

void CPU::AND_Immediate()
{
    DEBUG("0xE6 AND A,#");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);

    uint8_t val = registers.pc++;
    DEBUG("Operand");
    DEBUG_PRINT_REGISTER(val);

    uint8_t tReg = registers.a & val;
    if(tReg == 0x00) SetZeroFlag();
    ClearSubtractFlag();
    SetHalfCarryFlag();
    ClearCarryFlag();
    registers.a = tReg;
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
}

void CPU::OR_A()
{
    DEBUG("0xB7 OR A,A");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);

    uint8_t tReg = registers.a | registers.a;
    if(tReg == 0x00) SetZeroFlag();
    ClearSubtractFlag();
    ClearHalfCarryFlag();
    ClearCarryFlag();
    registers.a = tReg;
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
}

void CPU::OR_B()
{
    DEBUG("0xB0 OR A,B");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register B");
    DEBUG_PRINT_REGISTER(registers.b);

    uint8_t tReg = registers.a | registers.b;
    if(tReg == 0x00) SetZeroFlag();
    ClearSubtractFlag();
    ClearHalfCarryFlag();
    ClearCarryFlag();
    registers.a = tReg;
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
}

void CPU::OR_C()
{
    DEBUG("0xB1 OR A,C");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register C");
    DEBUG_PRINT_REGISTER(registers.c);

    uint8_t tReg = registers.a | registers.c;
    if(tReg == 0x00) SetZeroFlag();
    ClearSubtractFlag();
    ClearHalfCarryFlag();
    ClearCarryFlag();
    registers.a = tReg;
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
}

void CPU::OR_D()
{
    DEBUG("0xB2 OR A,D");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register D");
    DEBUG_PRINT_REGISTER(registers.d);

    uint8_t tReg = registers.a | registers.d;
    if(tReg == 0x00) SetZeroFlag();
    ClearSubtractFlag();
    ClearHalfCarryFlag();
    ClearCarryFlag();
    registers.a = tReg;
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
}

void CPU::OR_E()
{
    DEBUG("0xB3 OR A,E");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register E");
    DEBUG_PRINT_REGISTER(registers.e);

    uint8_t tReg = registers.a | registers.e;
    if(tReg == 0x00) SetZeroFlag();
    ClearSubtractFlag();
    ClearHalfCarryFlag();
    ClearCarryFlag();
    registers.a = tReg;
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
}

void CPU::OR_H()
{
    DEBUG("0xB4 OR A,H");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register H");
    DEBUG_PRINT_REGISTER(registers.h);

    uint8_t tReg = registers.a | registers.h;
    if(tReg == 0x00) SetZeroFlag();
    ClearSubtractFlag();
    ClearHalfCarryFlag();
    ClearCarryFlag();
    registers.a = tReg;
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
}

void CPU::OR_L()
{
    DEBUG("0xB5 OR A,L");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register L");
    DEBUG_PRINT_REGISTER(registers.l);

    uint8_t tReg = registers.a | registers.l;
    if(tReg == 0x00) SetZeroFlag();
    ClearSubtractFlag();
    ClearHalfCarryFlag();
    ClearCarryFlag();
    registers.a = tReg;
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
}

void CPU::OR_HL()
{
    DEBUG("0xB6 OR A,(HL)");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register H");
    DEBUG_PRINT_REGISTER(registers.h);
    DEBUG("Register L");
    DEBUG_PRINT_REGISTER(registers.l);

    uint8_t val = memory->Read(HL());
    DEBUG("Value at (HL)");
    DEBUG_PRINT_REGISTER(val);

    uint8_t tReg = registers.a | val;
    if(tReg == 0x00) SetZeroFlag();
    ClearSubtractFlag();
    ClearHalfCarryFlag();
    ClearCarryFlag();
    registers.a = tReg;
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
}

void CPU::OR_Immediate()
{
    DEBUG("0xF6 OR A,#");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);

    uint8_t val = registers.pc++;
    DEBUG("Operand");
    DEBUG_PRINT_REGISTER(val);

    uint8_t tReg = registers.a | val;
    if(tReg == 0x00) SetZeroFlag();
    ClearSubtractFlag();
    ClearHalfCarryFlag();
    ClearCarryFlag();
    registers.a = tReg;
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
}

void CPU::XOR_A()
{
    XOR(registers.a);
}

void CPU::XOR_B()
{
    XOR(registers.b);
}

void CPU::XOR_C()
{
    XOR(registers.c);
}

void CPU::XOR_D()
{
    XOR(registers.d);
}

void CPU::XOR_E()
{
    XOR(registers.e);
}

void CPU::XOR_H()
{
    XOR(registers.h);
}

void CPU::XOR_L()
{
    XOR(registers.l);
}

void CPU::XOR_HL()
{
    XOR(memory->Read(HL()));
}

void CPU::XOR_Immediate()
{
    XOR(registers.pc++);
}

void CPU::XOR(uint8_t op)
{
    uint8_t tReg = registers.a ^ op;
    if(tReg == 0x00) SetZeroFlag();
    ClearSubtractFlag();
    ClearHalfCarryFlag();
    ClearCarryFlag();
    registers.a = tReg;
}

void CPU::CP_A()
{
    CP(registers.a);
}

void CPU::CP_B()
{
    CP(registers.b);
}

void CPU::CP_C()
{
    CP(registers.c);
}

void CPU::CP_D()
{
    CP(registers.d);
}

void CPU::CP_E()
{
    CP(registers.e);
}

void CPU::CP_H()
{
    CP(registers.h);
}

void CPU::CP_L()
{
    CP(registers.l);
}

void CPU::CP_HL()
{
    CP(memory->Read(HL()));
}

void CPU::CP_Immediate()
{
    CP(registers.pc++);
}

void CPU::CP(uint8_t op)
{
    uint8_t res = registers.a - op;
    if(res == 0x00) SetZeroFlag();
    SetSubtractFlag();
    if(registers.a < op) SetCarryFlag();
    if(((registers.a & 0xF) - (op & 0xF)) < 0) SetHalfCarryFlag();
}

void CPU::INC_A()
{
    INC(registers.a);
}

void CPU::INC_B()
{
    INC(registers.b);
}

void CPU::INC_C()
{
    INC(registers.c);
}

void CPU::INC_D()
{
    INC(registers.d);
}

void CPU::INC_E()
{
    INC(registers.e);
}

void CPU::INC_H()
{
    INC(registers.h);
}

void CPU::INC_L()
{
    INC(registers.l);
}

void CPU::INC_HL()
{
    uint8_t v = memory->Read(HL());
    INC(v);
    memory->Write(HL(), v);
}

void CPU::INC(uint8_t& reg)
{
    uint8_t tReg = reg + 0x01;
    if(((tReg & 0xF) + (reg & 0xF)) & 0x10) SetHalfCarryFlag();
    reg = tReg;
    if(reg == 0x00) SetZeroFlag();
    ClearSubtractFlag();
}

void CPU::DEC_A()
{
    DEC(registers.a);
}

void CPU::DEC_B()
{
    DEC(registers.b);
}

void CPU::DEC_C()
{
    DEC(registers.c);
}

void CPU::DEC_D()
{
    DEC(registers.d);
}

void CPU::DEC_E()
{
    DEC(registers.e);
}

void CPU::DEC_H()
{
    DEC(registers.h);
}

void CPU::DEC_L()
{
    DEC(registers.l);
}

void CPU::DEC_HL()
{
    uint8_t v = memory->Read(HL());
    DEC(v);
    memory->Write(HL(), v);
}

void CPU::DEC(uint8_t& reg)
{
    uint8_t tReg = reg - 0x1;



    if(((tReg & 0xF) + (reg & 0xF)) & 0x10) SetHalfCarryFlag();
    reg = tReg;
    if(reg == 0x00) SetZeroFlag();
    SetSubtractFlag();
}

void CPU::ADD_BC_TO_HL()
{
    auto bc = BC();
    auto hl = HL();

    uint32_t result = bc + hl;
    ClearSubtractFlag();


}

void CPU::ADD_DE_TO_HL()
{

}

void CPU::ADD_HL_TO_HL()
{

}

void CPU::ADD_SP_TO_HL()
{

}
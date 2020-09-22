//
// Created by lorenzo on 8/16/20.
//

#ifndef ZURGBEMU_CPU_HPP
#define ZURGBEMU_CPU_HPP
#include <cstdint>
#include <iostream>
#include <functional>
#include <map>
#include "Memory.hpp"
#include "zurDebug.hpp"

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
    std::map<uint8_t, std::function<void(void)>> dispatchTable;

    void PopulateDispatchTable();

    void SetZeroFlag();
    void ClearZeroFlag();

    void SetSubtractFlag();
    void ClearSubtractFlag();

    void SetHalfCarryFlag();
    void ClearHalfCarryFlag();

    void SetCarryFlag();
    void ClearCarryFlag();

    uint16_t AF() {return (uint16_t)(registers.a << 8) | registers.f;}
    uint16_t BC() {return (uint16_t)(registers.b << 8) | registers.c;}
    uint16_t DE() {return (uint16_t)(registers.d << 8) | registers.e;}
    uint16_t HL() {return (uint16_t)(registers.h << 8) | registers.l;}

    void SetBC(uint16_t bc);
    void SetDE(uint16_t de);
    void SetHL(uint16_t hl);
    void SetSP(uint16_t sp);

    void HandleInstruction(uint8_t instruction);
    void NOP();
    void LDrn(uint8_t& r);
    void LDrr(uint8_t& r1, uint8_t& r2);
    void LDrHL(uint8_t& r);
    void LDHLr(uint8_t& r);
    void LDAn(uint8_t& r1);
    void LDAn(uint16_t addr);
    void LDnA(uint8_t& r);
    void LDnA(uint16_t addr);
    void LDAC();
    void LDCA();
    void LDAHLdec();
    void LDAHLinc();
    void LDHLAdec();
    void LDHLAinc();
    void LDHnA(uint8_t n);
    void LDHAn(uint8_t n);
    void LDBCnn(uint16_t i);
    void LDDEnn(uint16_t i);
    void LDHLnn(uint16_t i);
    void LDSPnn(uint16_t i);
    void LDSPHL();
    void LDHLSPn(uint8_t n);
    void LDnnSP(uint16_t nn);

    void PUSH_AF();
    void PUSH_BC();
    void PUSH_DE();
    void PUSH_HL();

    void POP_AF();
    void POP_BC();
    void POP_DE();
    void POP_HL();

    void ADD_A();
    void ADD_B();
    void ADD_C();
    void ADD_D();
    void ADD_E();
    void ADD_H();
    void ADD_L();
    void ADD_HL();
    void ADD_Immediate();

    void ADC_A();
    void ADC_B();
    void ADC_C();
    void ADC_D();
    void ADC_E();
    void ADC_H();
    void ADC_L();
    void ADC_HL();
    void ADC_Immediate();

    void SUB_A();
    void SUB_B();
    void SUB_C();
    void SUB_D();
    void SUB_E();
    void SUB_H();
    void SUB_L();
    void SUB_HL();
    void SUB_Immediate();

    void AND_A();
    void AND_B();
    void AND_C();
    void AND_D();
    void AND_E();
    void AND_H();
    void AND_L();
    void AND_HL();
    void AND_Immediate();

    void OR_A();
    void OR_B();
    void OR_C();
    void OR_D();
    void OR_E();
    void OR_H();
    void OR_L();
    void OR_HL();
    void OR_Immediate();

    Registers registers{};
    Memory* memory;
};


#endif //ZURGBEMU_CPU_HPP

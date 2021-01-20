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

enum class InterruptState
{
    Undefined,
    Enabled,
    Disabled,
    NeedToEnable,
    NeedToDisable
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
    std::map<uint8_t, std::function<void(void)>> extendedInstructionDispatchTable;

    void PopulateDispatchTable();
    void PopulateExtendedInstructionDispatchTable();

    void SetZeroFlag();
    void ClearZeroFlag();
    uint8_t GetZeroFlag();
    bool IsZeroFlagSet();

    void SetSubtractFlag();
    void ClearSubtractFlag();
    uint8_t GetSubtractFlag();
    bool IsSubtractFlagSet();

    void SetHalfCarryFlag();
    void ClearHalfCarryFlag();
    uint8_t GetHalfCarryFlag();
    bool IsHalfCarryFlagSet();

    void SetCarryFlag();
    void ClearCarryFlag();
    uint8_t GetCarryFlag();
    bool IsCarryFlagSet();

    uint16_t AF() {return (uint16_t)(registers.a << 8) | registers.f;}
    uint16_t BC() {return (uint16_t)(registers.b << 8) | registers.c;}
    uint16_t DE() {return (uint16_t)(registers.d << 8) | registers.e;}
    uint16_t HL() {return (uint16_t)(registers.h << 8) | registers.l;}

    void SetBC(uint16_t bc);
    void SetDE(uint16_t de);
    void SetHL(uint16_t hl);
    void SetSP(uint16_t sp);

    void HandleInstruction(uint8_t instruction);
    void HandleExtendedInstruction();

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
    void PUSH(uint16_t op);

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
    void ADD(uint8_t op);

    void ADC_A();
    void ADC_B();
    void ADC_C();
    void ADC_D();
    void ADC_E();
    void ADC_H();
    void ADC_L();
    void ADC_HL();
    void ADC_Immediate();
    void ADC(uint8_t op);

    void SUB_A();
    void SUB_B();
    void SUB_C();
    void SUB_D();
    void SUB_E();
    void SUB_H();
    void SUB_L();
    void SUB_HL();
    void SUB_Immediate();
    void SUB(uint8_t op);

    void SBC_A();
    void SBC_B();
    void SBC_C();
    void SBC_D();
    void SBC_E();
    void SBC_H();
    void SBC_L();
    void SBC_HL();
    void SBC_Immediate();
    void SBC(uint8_t op);

    void AND_A();
    void AND_B();
    void AND_C();
    void AND_D();
    void AND_E();
    void AND_H();
    void AND_L();
    void AND_HL();
    void AND_Immediate();
    void AND(uint8_t op);

    void OR_A();
    void OR_B();
    void OR_C();
    void OR_D();
    void OR_E();
    void OR_H();
    void OR_L();
    void OR_HL();
    void OR_Immediate();
    void OR(uint8_t op);

    void XOR_A();
    void XOR_B();
    void XOR_C();
    void XOR_D();
    void XOR_E();
    void XOR_H();
    void XOR_L();
    void XOR_HL();
    void XOR_Immediate();
    void XOR(uint8_t op);

    void CP_A();
    void CP_B();
    void CP_C();
    void CP_D();
    void CP_E();
    void CP_H();
    void CP_L();
    void CP_HL();
    void CP_Immediate();
    void CP(uint8_t op);

    void INC_A();
    void INC_B();
    void INC_C();
    void INC_D();
    void INC_E();
    void INC_H();
    void INC_L();
    void INC_VALUE_AT_HL();
    void INC(uint8_t& reg);

    void DEC_A();
    void DEC_B();
    void DEC_C();
    void DEC_D();
    void DEC_E();
    void DEC_H();
    void DEC_L();
    void DEC_VALUE_AT_HL();
    void DEC(uint8_t& reg);

    void ADD_BC_TO_HL();
    void ADD_DE_TO_HL();
    void ADD_HL_TO_HL();
    void ADD_SP_TO_HL();
    void ADD_TO_HL(uint16_t op);

    void ADD_SP();

    void INC_BC();
    void INC_DE();
    void INC_HL();
    void INC_SP();

    void DEC_BC();
    void DEC_DE();
    void DEC_HL();
    void DEC_SP();

    void SWAP_A();
    void SWAP_B();
    void SWAP_C();
    void SWAP_D();
    void SWAP_E();
    void SWAP_H();
    void SWAP_L();
    void SWAP_VALUE_AT_HL();
    void SWAP(uint8_t& op);
  
    void DAA();
    void CPL();
    void CCF();
    void SCF();
    void HALT();
    void STOP();

    void DI();
    void EI();

    void RLCA();
    void RLA();
    void RRCA();
    void RRA();

    void RLC_A();
    void RLC_B();
    void RLC_C();
    void RLC_D();
    void RLC_E();
    void RLC_H();
    void RLC_L();
    void RLC_VALUE_AT_HL();
    void RLC_n(uint8_t& op);

    void RL_A();
    void RL_B();
    void RL_C();
    void RL_D();
    void RL_E();
    void RL_H();
    void RL_L();
    void RL_VALUE_AT_HL();
    void RL_n(uint8_t& op);

    void RR_A();
    void RR_B();
    void RR_C();
    void RR_D();
    void RR_E();
    void RR_H();
    void RR_L();
    void RR_VALUE_AT_HL();
    void RR_n(uint8_t& op);

    void SLA_A();
    void SLA_B();
    void SLA_C();
    void SLA_D();
    void SLA_E();
    void SLA_H();
    void SLA_L();
    void SLA_VALUE_AT_HL();
    void SLA_n(uint8_t& op);

    void SRA_A();
    void SRA_B();
    void SRA_C();
    void SRA_D();
    void SRA_E();
    void SRA_H();
    void SRA_L();
    void SRA_VALUE_AT_HL();
    void SRA_n(uint8_t& op);

    void SRL_A();
    void SRL_B();
    void SRL_C();
    void SRL_D();
    void SRL_E();
    void SRL_H();
    void SRL_L();
    void SRL_VALUE_AT_HL();
    void SRL_n(uint8_t& op);

    void BIT_0_A();
    void BIT_0_B();
    void BIT_0_C();
    void BIT_0_D();
    void BIT_0_E();
    void BIT_0_H();
    void BIT_0_L();
    void BIT_0_VALUE_AT_HL();

    void BIT_1_A();
    void BIT_1_B();
    void BIT_1_C();
    void BIT_1_D();
    void BIT_1_E();
    void BIT_1_H();
    void BIT_1_L();
    void BIT_1_VALUE_AT_HL();

    void BIT_2_A();
    void BIT_2_B();
    void BIT_2_C();
    void BIT_2_D();
    void BIT_2_E();
    void BIT_2_H();
    void BIT_2_L();
    void BIT_2_VALUE_AT_HL();

    void BIT_3_A();
    void BIT_3_B();
    void BIT_3_C();
    void BIT_3_D();
    void BIT_3_E();
    void BIT_3_H();
    void BIT_3_L();
    void BIT_3_VALUE_AT_HL();

    void BIT_4_A();
    void BIT_4_B();
    void BIT_4_C();
    void BIT_4_D();
    void BIT_4_E();
    void BIT_4_H();
    void BIT_4_L();
    void BIT_4_VALUE_AT_HL();

    void BIT_5_A();
    void BIT_5_B();
    void BIT_5_C();
    void BIT_5_D();
    void BIT_5_E();
    void BIT_5_H();
    void BIT_5_L();
    void BIT_5_VALUE_AT_HL();

    void BIT_6_A();
    void BIT_6_B();
    void BIT_6_C();
    void BIT_6_D();
    void BIT_6_E();
    void BIT_6_H();
    void BIT_6_L();
    void BIT_6_VALUE_AT_HL();

    void BIT_7_A();
    void BIT_7_B();
    void BIT_7_C();
    void BIT_7_D();
    void BIT_7_E();
    void BIT_7_H();
    void BIT_7_L();
    void BIT_7_VALUE_AT_HL();

    void BIT_b_N(uint8_t bitPos, uint8_t& op);

    void SET_0_A();
    void SET_0_B();
    void SET_0_C();
    void SET_0_D();
    void SET_0_E();
    void SET_0_H();
    void SET_0_L();
    void SET_0_VALUE_AT_HL();

    void SET_1_A();
    void SET_1_B();
    void SET_1_C();
    void SET_1_D();
    void SET_1_E();
    void SET_1_H();
    void SET_1_L();
    void SET_1_VALUE_AT_HL();

    void SET_2_A();
    void SET_2_B();
    void SET_2_C();
    void SET_2_D();
    void SET_2_E();
    void SET_2_H();
    void SET_2_L();
    void SET_2_VALUE_AT_HL();

    void SET_3_A();
    void SET_3_B();
    void SET_3_C();
    void SET_3_D();
    void SET_3_E();
    void SET_3_H();
    void SET_3_L();
    void SET_3_VALUE_AT_HL();

    void SET_4_A();
    void SET_4_B();
    void SET_4_C();
    void SET_4_D();
    void SET_4_E();
    void SET_4_H();
    void SET_4_L();
    void SET_4_VALUE_AT_HL();

    void SET_5_A();
    void SET_5_B();
    void SET_5_C();
    void SET_5_D();
    void SET_5_E();
    void SET_5_H();
    void SET_5_L();
    void SET_5_VALUE_AT_HL();

    void SET_6_A();
    void SET_6_B();
    void SET_6_C();
    void SET_6_D();
    void SET_6_E();
    void SET_6_H();
    void SET_6_L();
    void SET_6_VALUE_AT_HL();

    void SET_7_A();
    void SET_7_B();
    void SET_7_C();
    void SET_7_D();
    void SET_7_E();
    void SET_7_H();
    void SET_7_L();
    void SET_7_VALUE_AT_HL();

    void SET_b_N(uint8_t bitPos, uint8_t& op);

    Registers registers{};
    Memory* memory;
    InterruptState interruptState;
};

#endif //ZURGBEMU_CPU_HPP

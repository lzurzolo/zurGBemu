//
// Created by lorenzo on 8/16/20.
//

#include "CPU.hpp"

const uint8_t SUBTRACT_FLAG_BIT_MASK = 0b01000000;
const uint8_t ZERO_FLAG_BIT_MASK = 0b10000000;
const uint8_t CARRY_FLAG_BIT_MASK =- 0b00010000;
const uint8_t HALF_CARRY_FLAG_BIT_MASK = 0b00100000;
const uint8_t DISABLE_INTERRUPT_OPCODE = 0xF3;
const uint8_t ENABLE_INTERRUPT_OPCODE = 0xFB;

CPU::CPU(Memory* m)
: memory(m)
{
    Reset();
    PopulateDispatchTable();
    PopulateExtendedInstructionDispatchTable();
}

CPU::~CPU()
= default;

void CPU::PopulateDispatchTable()
{
    dispatchTable.insert(std::make_pair(0x00, std::bind(&CPU::NOP, this)));
    dispatchTable.insert(std::make_pair(0x03, std::bind(&CPU::INC_BC, this)));
    dispatchTable.insert(std::make_pair(0x04, std::bind(&CPU::INC_B, this)));
    dispatchTable.insert(std::make_pair(0x05, std::bind(&CPU::DEC_B, this)));
    dispatchTable.insert(std::make_pair(0x07, std::bind(&CPU::RLCA, this)));
    dispatchTable.insert(std::make_pair(0x09, std::bind(&CPU::ADD_BC_TO_HL, this)));
    dispatchTable.insert(std::make_pair(0x0B, std::bind(&CPU::DEC_BC, this)));
    dispatchTable.insert(std::make_pair(0x0C, std::bind(&CPU::INC_C, this)));
    dispatchTable.insert(std::make_pair(0x0D, std::bind(&CPU::DEC_C, this)));
    dispatchTable.insert(std::make_pair(0x0F, std::bind(&CPU::RRCA, this)));
    dispatchTable.insert(std::make_pair(0x13, std::bind(&CPU::INC_DE, this)));
    dispatchTable.insert(std::make_pair(0x14, std::bind(&CPU::INC_D, this)));
    dispatchTable.insert(std::make_pair(0x15, std::bind(&CPU::DEC_D, this)));
    dispatchTable.insert(std::make_pair(0x17, std::bind(&CPU::RLA, this)));
    dispatchTable.insert(std::make_pair(0x18, std::bind(&CPU::JR_n, this)));
    dispatchTable.insert(std::make_pair(0x19, std::bind(&CPU::ADD_DE_TO_HL, this)));
    dispatchTable.insert(std::make_pair(0x20, std::bind(&CPU::JR_NZ_n, this)));
    dispatchTable.insert(std::make_pair(0x1B, std::bind(&CPU::DEC_DE, this)));
    dispatchTable.insert(std::make_pair(0x1C, std::bind(&CPU::INC_E, this)));
    dispatchTable.insert(std::make_pair(0x1D, std::bind(&CPU::DEC_E, this)));
    dispatchTable.insert(std::make_pair(0x1F, std::bind(&CPU::RRA, this)));
    dispatchTable.insert(std::make_pair(0x2B, std::bind(&CPU::DEC_HL, this)));
    dispatchTable.insert(std::make_pair(0x23, std::bind(&CPU::INC_HL, this)));
    dispatchTable.insert(std::make_pair(0x27, std::bind(&CPU::DAA, this)));
    dispatchTable.insert(std::make_pair(0x28, std::bind(&CPU::JR_Z_n, this)));
    dispatchTable.insert(std::make_pair(0x29, std::bind(&CPU::ADD_HL_TO_HL, this)));
    dispatchTable.insert(std::make_pair(0x24, std::bind(&CPU::INC_H, this)));
    dispatchTable.insert(std::make_pair(0x25, std::bind(&CPU::DEC_H, this)));
    dispatchTable.insert(std::make_pair(0x2C, std::bind(&CPU::INC_L, this)));
    dispatchTable.insert(std::make_pair(0x2D, std::bind(&CPU::DEC_L, this)));
    dispatchTable.insert(std::make_pair(0x2F, std::bind(&CPU::CPL, this)));
    dispatchTable.insert(std::make_pair(0x30, std::bind(&CPU::JR_NC_n, this)));
    dispatchTable.insert(std::make_pair(0x33, std::bind(&CPU::INC_SP, this)));
    dispatchTable.insert(std::make_pair(0x34, std::bind(&CPU::INC_VALUE_AT_HL, this)));
    dispatchTable.insert(std::make_pair(0x35, std::bind(&CPU::DEC_VALUE_AT_HL, this)));
    dispatchTable.insert(std::make_pair(0x37, std::bind(&CPU::SCF, this)));
    dispatchTable.insert(std::make_pair(0x38, std::bind(&CPU::JR_C_n, this)));
    dispatchTable.insert(std::make_pair(0x39, std::bind(&CPU::ADD_SP_TO_HL, this)));
    dispatchTable.insert(std::make_pair(0x3B, std::bind(&CPU::DEC_SP, this)));
    dispatchTable.insert(std::make_pair(0x3C, std::bind(&CPU::INC_A, this)));
    dispatchTable.insert(std::make_pair(0x3D, std::bind(&CPU::DEC_A, this)));
    dispatchTable.insert(std::make_pair(0x3F, std::bind(&CPU::CCF, this)));
    dispatchTable.insert(std::make_pair(0x76, std::bind(&CPU::HALT, this)));
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
    dispatchTable.insert(std::make_pair(0xC0, std::bind(&CPU::RET_NZ, this)));
    dispatchTable.insert(std::make_pair(0xC1, std::bind(&CPU::POP_BC, this)));
    dispatchTable.insert(std::make_pair(0xC2, std::bind(&CPU::JP_NZ_nn, this)));
    dispatchTable.insert(std::make_pair(0xC3, std::bind(&CPU::JP_nn, this)));
    dispatchTable.insert(std::make_pair(0xC4, std::bind(&CPU::CALL_NZ_nn, this)));
    dispatchTable.insert(std::make_pair(0xC5, std::bind(&CPU::PUSH_BC, this)));
    dispatchTable.insert(std::make_pair(0xC6, std::bind(&CPU::ADD_Immediate, this)));
    dispatchTable.insert(std::make_pair(0xC7, std::bind(&CPU::RST_00H, this)));
    dispatchTable.insert(std::make_pair(0xC8, std::bind(&CPU::RET_Z, this)));
    dispatchTable.insert(std::make_pair(0xC9, std::bind(&CPU::RET, this)));
    dispatchTable.insert(std::make_pair(0xCA, std::bind(&CPU::JP_Z_nn, this)));
    dispatchTable.insert(std::make_pair(0xCB, std::bind(&CPU::HandleExtendedInstruction, this)));
    dispatchTable.insert(std::make_pair(0xCC, std::bind(&CPU::CALL_Z_nn, this)));
    dispatchTable.insert(std::make_pair(0xCD, std::bind(&CPU::CALL_nn, this)));
    dispatchTable.insert(std::make_pair(0xCE, std::bind(&CPU::ADC_Immediate, this)));
    dispatchTable.insert(std::make_pair(0xCF, std::bind(&CPU::RST_08H, this)));
    dispatchTable.insert(std::make_pair(0xD0, std::bind(&CPU::RET_NC, this)));
    dispatchTable.insert(std::make_pair(0xD1, std::bind(&CPU::POP_DE, this)));
    dispatchTable.insert(std::make_pair(0xD2, std::bind(&CPU::JP_NC_nn, this)));
    dispatchTable.insert(std::make_pair(0xD4, std::bind(&CPU::CALL_NC_nn, this)));
    dispatchTable.insert(std::make_pair(0xD5, std::bind(&CPU::PUSH_DE, this)));
    dispatchTable.insert(std::make_pair(0xD6, std::bind(&CPU::SUB_Immediate, this)));
    dispatchTable.insert(std::make_pair(0xD7, std::bind(&CPU::RST_10H, this)));
    dispatchTable.insert(std::make_pair(0xD8, std::bind(&CPU::RET_C, this)));
    dispatchTable.insert(std::make_pair(0xD9, std::bind(&CPU::RETI, this)));
    dispatchTable.insert(std::make_pair(0xDA, std::bind(&CPU::JP_C_nn, this)));
    dispatchTable.insert(std::make_pair(0xDC, std::bind(&CPU::CALL_C_nn, this)));
    dispatchTable.insert(std::make_pair(0xDE, std::bind(&CPU::SBC_Immediate, this)));
    dispatchTable.insert(std::make_pair(0xDF, std::bind(&CPU::RST_18H, this)));
    dispatchTable.insert(std::make_pair(0xE1, std::bind(&CPU::POP_HL, this)));
    dispatchTable.insert(std::make_pair(0xE5, std::bind(&CPU::PUSH_HL, this)));
    dispatchTable.insert(std::make_pair(0xE6, std::bind(&CPU::AND_Immediate, this)));
    dispatchTable.insert(std::make_pair(0xE7, std::bind(&CPU::RST_20H, this)));
    dispatchTable.insert(std::make_pair(0xE8, std::bind(&CPU::ADD_SP, this)));
    dispatchTable.insert(std::make_pair(0xE9, std::bind(&CPU::JP_HL, this)));
    dispatchTable.insert(std::make_pair(0xEE, std::bind(&CPU::XOR_Immediate, this)));
    dispatchTable.insert(std::make_pair(0xEF, std::bind(&CPU::RST_28H, this)));
    dispatchTable.insert(std::make_pair(0xF1, std::bind(&CPU::POP_AF, this)));
    dispatchTable.insert(std::make_pair(0xF3, std::bind(&CPU::DI, this)));
    dispatchTable.insert(std::make_pair(0xF5, std::bind(&CPU::PUSH_AF, this)));
    dispatchTable.insert(std::make_pair(0xF6, std::bind(&CPU::OR_Immediate, this)));
    dispatchTable.insert(std::make_pair(0xF7, std::bind(&CPU::RST_30H, this)));
    dispatchTable.insert(std::make_pair(0xFB, std::bind(&CPU::EI, this)));
    dispatchTable.insert(std::make_pair(0xFE, std::bind(&CPU::CP_Immediate, this)));
    dispatchTable.insert(std::make_pair(0xFF, std::bind(&CPU::RST_38H, this)));
}

void CPU::PopulateExtendedInstructionDispatchTable()
{
    extendedInstructionDispatchTable.insert(std::make_pair(0x00, std::bind(&CPU::RLC_B, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x01, std::bind(&CPU::RLC_C, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x02, std::bind(&CPU::RLC_D, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x03, std::bind(&CPU::RLC_E, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x04, std::bind(&CPU::RLC_H, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x05, std::bind(&CPU::RLC_L, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x06, std::bind(&CPU::RLC_VALUE_AT_HL, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x07, std::bind(&CPU::RLC_A, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x10, std::bind(&CPU::RL_B, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x11, std::bind(&CPU::RL_C, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x12, std::bind(&CPU::RL_D, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x13, std::bind(&CPU::RL_E, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x14, std::bind(&CPU::RL_H, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x15, std::bind(&CPU::RL_L, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x16, std::bind(&CPU::RL_VALUE_AT_HL, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x17, std::bind(&CPU::RL_A, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x18, std::bind(&CPU::RR_B, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x19, std::bind(&CPU::RR_C, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x1A, std::bind(&CPU::RR_D, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x1B, std::bind(&CPU::RR_E, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x1C, std::bind(&CPU::RR_H, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x1D, std::bind(&CPU::RR_L, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x1E, std::bind(&CPU::RR_VALUE_AT_HL, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x1F, std::bind(&CPU::RR_A, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x20, std::bind(&CPU::SLA_B, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x21, std::bind(&CPU::SLA_C, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x22, std::bind(&CPU::SLA_D, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x23, std::bind(&CPU::SLA_E, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x24, std::bind(&CPU::SLA_H, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x25, std::bind(&CPU::SLA_L, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x26, std::bind(&CPU::SLA_VALUE_AT_HL, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x27, std::bind(&CPU::SLA_A, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x28, std::bind(&CPU::SRA_B, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x29, std::bind(&CPU::SRA_C, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x2A, std::bind(&CPU::SRA_D, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x2B, std::bind(&CPU::SRA_E, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x2C, std::bind(&CPU::SRA_H, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x2D, std::bind(&CPU::SRA_L, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x2E, std::bind(&CPU::SRA_VALUE_AT_HL, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x2F, std::bind(&CPU::SRA_A, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x30, std::bind(&CPU::SWAP_B, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x31, std::bind(&CPU::SWAP_C, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x32, std::bind(&CPU::SWAP_D, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x33, std::bind(&CPU::SWAP_E, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x34, std::bind(&CPU::SWAP_H, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x35, std::bind(&CPU::SWAP_L, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x36, std::bind(&CPU::SWAP_VALUE_AT_HL, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x37, std::bind(&CPU::SWAP_A, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x38, std::bind(&CPU::SRL_B, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x39, std::bind(&CPU::SRL_C, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x3A, std::bind(&CPU::SRL_D, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x3B, std::bind(&CPU::SRL_E, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x3C, std::bind(&CPU::SRL_H, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x3D, std::bind(&CPU::SRL_L, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x3E, std::bind(&CPU::SRL_VALUE_AT_HL, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x3F, std::bind(&CPU::SRL_A, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x40, std::bind(&CPU::BIT_0_B, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x41, std::bind(&CPU::BIT_0_C, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x42, std::bind(&CPU::BIT_0_D, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x43, std::bind(&CPU::BIT_0_E, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x44, std::bind(&CPU::BIT_0_H, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x45, std::bind(&CPU::BIT_0_L, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x46, std::bind(&CPU::BIT_0_VALUE_AT_HL, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x47, std::bind(&CPU::BIT_0_A, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x48, std::bind(&CPU::BIT_1_B, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x49, std::bind(&CPU::BIT_1_C, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x4A, std::bind(&CPU::BIT_1_D, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x4B, std::bind(&CPU::BIT_1_E, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x4C, std::bind(&CPU::BIT_1_H, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x4D, std::bind(&CPU::BIT_1_L, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x4E, std::bind(&CPU::BIT_1_VALUE_AT_HL, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x4F, std::bind(&CPU::BIT_1_A, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x50, std::bind(&CPU::BIT_2_B, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x51, std::bind(&CPU::BIT_2_C, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x52, std::bind(&CPU::BIT_2_D, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x53, std::bind(&CPU::BIT_2_E, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x54, std::bind(&CPU::BIT_2_H, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x55, std::bind(&CPU::BIT_2_L, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x56, std::bind(&CPU::BIT_2_VALUE_AT_HL, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x57, std::bind(&CPU::BIT_2_A, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x58, std::bind(&CPU::BIT_3_B, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x59, std::bind(&CPU::BIT_3_C, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x5A, std::bind(&CPU::BIT_3_D, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x5B, std::bind(&CPU::BIT_3_E, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x5C, std::bind(&CPU::BIT_3_H, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x5D, std::bind(&CPU::BIT_3_L, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x5E, std::bind(&CPU::BIT_3_VALUE_AT_HL, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x5F, std::bind(&CPU::BIT_3_A, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x60, std::bind(&CPU::BIT_4_B, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x61, std::bind(&CPU::BIT_4_C, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x62, std::bind(&CPU::BIT_4_D, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x63, std::bind(&CPU::BIT_4_E, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x64, std::bind(&CPU::BIT_4_H, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x65, std::bind(&CPU::BIT_4_L, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x66, std::bind(&CPU::BIT_4_VALUE_AT_HL, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x67, std::bind(&CPU::BIT_4_A, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x68, std::bind(&CPU::BIT_5_B, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x69, std::bind(&CPU::BIT_5_C, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x6A, std::bind(&CPU::BIT_5_D, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x6B, std::bind(&CPU::BIT_5_E, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x6C, std::bind(&CPU::BIT_5_H, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x6D, std::bind(&CPU::BIT_5_L, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x6E, std::bind(&CPU::BIT_5_VALUE_AT_HL, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x6F, std::bind(&CPU::BIT_5_A, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x70, std::bind(&CPU::BIT_6_B, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x71, std::bind(&CPU::BIT_6_C, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x72, std::bind(&CPU::BIT_6_D, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x73, std::bind(&CPU::BIT_6_E, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x74, std::bind(&CPU::BIT_6_H, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x75, std::bind(&CPU::BIT_6_L, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x76, std::bind(&CPU::BIT_6_VALUE_AT_HL, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x77, std::bind(&CPU::BIT_6_A, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x78, std::bind(&CPU::BIT_7_B, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x79, std::bind(&CPU::BIT_7_C, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x7A, std::bind(&CPU::BIT_7_D, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x7B, std::bind(&CPU::BIT_7_E, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x7C, std::bind(&CPU::BIT_7_H, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x7D, std::bind(&CPU::BIT_7_L, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x7E, std::bind(&CPU::BIT_7_VALUE_AT_HL, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x7F, std::bind(&CPU::BIT_7_A, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x80, std::bind(&CPU::RES_0_B, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x81, std::bind(&CPU::RES_0_C, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x82, std::bind(&CPU::RES_0_D, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x83, std::bind(&CPU::RES_0_E, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x84, std::bind(&CPU::RES_0_H, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x85, std::bind(&CPU::RES_0_L, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x86, std::bind(&CPU::RES_0_VALUE_AT_HL, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x87, std::bind(&CPU::RES_0_A, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x88, std::bind(&CPU::RES_1_B, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x89, std::bind(&CPU::RES_1_C, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x8A, std::bind(&CPU::RES_1_D, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x8B, std::bind(&CPU::RES_1_E, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x8C, std::bind(&CPU::RES_1_H, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x8D, std::bind(&CPU::RES_1_L, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x8E, std::bind(&CPU::RES_1_VALUE_AT_HL, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x8F, std::bind(&CPU::RES_1_A, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x90, std::bind(&CPU::RES_2_B, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x91, std::bind(&CPU::RES_2_C, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x92, std::bind(&CPU::RES_2_D, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x93, std::bind(&CPU::RES_2_E, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x94, std::bind(&CPU::RES_2_H, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x95, std::bind(&CPU::RES_2_L, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x96, std::bind(&CPU::RES_2_VALUE_AT_HL, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x97, std::bind(&CPU::RES_2_A, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x98, std::bind(&CPU::RES_3_B, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x99, std::bind(&CPU::RES_3_C, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x9A, std::bind(&CPU::RES_3_D, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x9B, std::bind(&CPU::RES_3_E, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x9C, std::bind(&CPU::RES_3_H, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x9D, std::bind(&CPU::RES_3_L, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x9E, std::bind(&CPU::RES_3_VALUE_AT_HL, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0x9F, std::bind(&CPU::RES_3_A, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xA0, std::bind(&CPU::RES_4_B, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xA1, std::bind(&CPU::RES_4_C, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xA2, std::bind(&CPU::RES_4_D, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xA3, std::bind(&CPU::RES_4_E, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xA4, std::bind(&CPU::RES_4_H, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xA5, std::bind(&CPU::RES_4_L, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xA6, std::bind(&CPU::RES_4_VALUE_AT_HL, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xA7, std::bind(&CPU::RES_4_A, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xA8, std::bind(&CPU::RES_5_B, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xA9, std::bind(&CPU::RES_5_C, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xAA, std::bind(&CPU::RES_5_D, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xAB, std::bind(&CPU::RES_5_E, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xAC, std::bind(&CPU::RES_5_H, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xAD, std::bind(&CPU::RES_5_L, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xAE, std::bind(&CPU::RES_5_VALUE_AT_HL, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xAF, std::bind(&CPU::RES_5_A, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xB0, std::bind(&CPU::RES_6_B, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xB1, std::bind(&CPU::RES_6_C, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xB2, std::bind(&CPU::RES_6_D, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xB3, std::bind(&CPU::RES_6_E, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xB4, std::bind(&CPU::RES_6_H, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xB5, std::bind(&CPU::RES_6_L, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xB6, std::bind(&CPU::RES_6_VALUE_AT_HL, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xB7, std::bind(&CPU::RES_6_A, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xB8, std::bind(&CPU::RES_7_B, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xB9, std::bind(&CPU::RES_7_C, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xBA, std::bind(&CPU::RES_7_D, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xBB, std::bind(&CPU::RES_7_E, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xBC, std::bind(&CPU::RES_7_H, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xBD, std::bind(&CPU::RES_7_L, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xBE, std::bind(&CPU::RES_7_VALUE_AT_HL, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xBF, std::bind(&CPU::RES_7_A, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xC0, std::bind(&CPU::SET_0_B, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xC1, std::bind(&CPU::SET_0_C, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xC2, std::bind(&CPU::SET_0_D, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xC3, std::bind(&CPU::SET_0_E, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xC4, std::bind(&CPU::SET_0_H, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xC5, std::bind(&CPU::SET_0_L, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xC6, std::bind(&CPU::SET_0_VALUE_AT_HL, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xC7, std::bind(&CPU::SET_0_A, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xC8, std::bind(&CPU::SET_1_B, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xC9, std::bind(&CPU::SET_1_C, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xCA, std::bind(&CPU::SET_1_D, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xCB, std::bind(&CPU::SET_1_E, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xCC, std::bind(&CPU::SET_1_H, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xCD, std::bind(&CPU::SET_1_L, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xCE, std::bind(&CPU::SET_1_VALUE_AT_HL, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xCF, std::bind(&CPU::SET_1_A, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xD0, std::bind(&CPU::SET_2_B, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xD1, std::bind(&CPU::SET_2_C, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xD2, std::bind(&CPU::SET_2_D, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xD3, std::bind(&CPU::SET_2_E, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xD4, std::bind(&CPU::SET_2_H, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xD5, std::bind(&CPU::SET_2_L, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xD6, std::bind(&CPU::SET_2_VALUE_AT_HL, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xD7, std::bind(&CPU::SET_2_A, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xD8, std::bind(&CPU::SET_3_B, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xD9, std::bind(&CPU::SET_3_C, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xDA, std::bind(&CPU::SET_3_D, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xDB, std::bind(&CPU::SET_3_E, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xDC, std::bind(&CPU::SET_3_H, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xDD, std::bind(&CPU::SET_3_L, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xDE, std::bind(&CPU::SET_3_VALUE_AT_HL, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xDF, std::bind(&CPU::SET_3_A, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xE0, std::bind(&CPU::SET_4_B, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xE1, std::bind(&CPU::SET_4_C, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xE2, std::bind(&CPU::SET_4_D, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xE3, std::bind(&CPU::SET_4_E, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xE4, std::bind(&CPU::SET_4_H, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xE5, std::bind(&CPU::SET_4_L, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xE6, std::bind(&CPU::SET_4_VALUE_AT_HL, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xE7, std::bind(&CPU::SET_4_A, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xE8, std::bind(&CPU::SET_5_B, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xE9, std::bind(&CPU::SET_5_C, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xEA, std::bind(&CPU::SET_5_D, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xEB, std::bind(&CPU::SET_5_E, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xEC, std::bind(&CPU::SET_5_H, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xED, std::bind(&CPU::SET_5_L, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xEE, std::bind(&CPU::SET_5_VALUE_AT_HL, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xEF, std::bind(&CPU::SET_5_A, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xF0, std::bind(&CPU::SET_6_B, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xF1, std::bind(&CPU::SET_6_C, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xF2, std::bind(&CPU::SET_6_D, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xF3, std::bind(&CPU::SET_6_E, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xF4, std::bind(&CPU::SET_6_H, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xF5, std::bind(&CPU::SET_6_L, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xF6, std::bind(&CPU::SET_6_VALUE_AT_HL, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xF7, std::bind(&CPU::SET_6_A, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xF8, std::bind(&CPU::SET_7_B, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xF9, std::bind(&CPU::SET_7_C, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xFA, std::bind(&CPU::SET_7_D, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xFB, std::bind(&CPU::SET_7_E, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xFC, std::bind(&CPU::SET_7_H, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xFD, std::bind(&CPU::SET_7_L, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xFE, std::bind(&CPU::SET_7_VALUE_AT_HL, this)));
    extendedInstructionDispatchTable.insert(std::make_pair(0xFF, std::bind(&CPU::SET_7_A, this)));
}

void CPU::SetZeroFlag()
{
    registers.f |= ZERO_FLAG_BIT_MASK;
}

void CPU::ClearZeroFlag()
{
    registers.f &= ~ZERO_FLAG_BIT_MASK;
}

uint8_t CPU::GetZeroFlag()
{
    return registers.f & ZERO_FLAG_BIT_MASK;
}

bool CPU::IsZeroFlagSet()
{
    return registers.f & ZERO_FLAG_BIT_MASK;
}

void CPU::SetSubtractFlag()
{
    registers.f |= SUBTRACT_FLAG_BIT_MASK;
}

void CPU::ClearSubtractFlag()
{
    registers.f &= ~SUBTRACT_FLAG_BIT_MASK;
}

uint8_t CPU::GetSubtractFlag()
{
    return registers.f & SUBTRACT_FLAG_BIT_MASK;
}

bool CPU::IsSubtractFlagSet()
{
    return registers.f & SUBTRACT_FLAG_BIT_MASK;
}

void CPU::SetHalfCarryFlag()
{
    registers.f |= HALF_CARRY_FLAG_BIT_MASK;
}

void CPU::ClearHalfCarryFlag()
{
    registers.f &= ~HALF_CARRY_FLAG_BIT_MASK;
}

uint8_t CPU::GetHalfCarryFlag()
{
    return registers.f & HALF_CARRY_FLAG_BIT_MASK;
}
    
bool CPU::IsHalfCarryFlagSet()
{
    return registers.f & HALF_CARRY_FLAG_BIT_MASK;
}

void CPU::SetCarryFlag()
{
    registers.f |= CARRY_FLAG_BIT_MASK;
}

void CPU::ClearCarryFlag()
{
    registers.f &= ~CARRY_FLAG_BIT_MASK;
}

uint8_t CPU::GetCarryFlag()
{
    return registers.f & CARRY_FLAG_BIT_MASK;
}

bool CPU::IsCarryFlagSet()
{
    return registers.f & CARRY_FLAG_BIT_MASK;
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

        // the EI and DI instructions respectively enable and disable interrupts after the next instruction
        // following the EI or DI instruction has been executed
        if(interruptState == InterruptState::NeedToDisable && instruction != DISABLE_INTERRUPT_OPCODE)
        {
            interruptState = InterruptState::Disabled;
        }
        else if(interruptState == InterruptState::NeedToEnable && instruction != ENABLE_INTERRUPT_OPCODE)
        {
            interruptState = InterruptState::Enabled;
        }
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

void CPU::HandleExtendedInstruction()
{
    auto instruction = registers.pc++;
    if (extendedInstructionDispatchTable.find(instruction) == extendedInstructionDispatchTable.end()) 
    {

    }
    else 
    {
        extendedInstructionDispatchTable[instruction]();
    }
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

    ADC(registers.a);
}

void CPU::ADC_B()
{
    DEBUG("0x88 ADC A,B");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register B");
    DEBUG_PRINT_REGISTER(registers.b);

    ADC(registers.b);
}

void CPU::ADC_C()
{
    DEBUG("0x89 ADC A,C");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register C");
    DEBUG_PRINT_REGISTER(registers.c);

    ADC(registers.c);
}

void CPU::ADC_D()
{
    DEBUG("0x8A ADC A,D");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register D");
    DEBUG_PRINT_REGISTER(registers.d);

    ADC(registers.d);
}

void CPU::ADC_E()
{
    DEBUG("0x8B ADC A,E");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register E");
    DEBUG_PRINT_REGISTER(registers.e);

    ADC(registers.e);
}

void CPU::ADC_H()
{
    DEBUG("0x8C ADC A,H");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register H");
    DEBUG_PRINT_REGISTER(registers.h);

    ADC(registers.h);
}

void CPU::ADC_L()
{
    DEBUG("0x8D ADC A,L");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register L");
    DEBUG_PRINT_REGISTER(registers.l);

    ADC(registers.l);
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

    ADC(memory->Read(HL()));
}

void CPU::ADC_Immediate()
{
    DEBUG("0xCE ADC A,#");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    ADC(registers.pc++);
}

void CPU::ADC(uint8_t op)
{
    DEBUG("OPERAND");
    DEBUG_PRINT_REGISTER(op);
    DEBUG("Carry");
    uint8_t carryFlag = GetCarryFlag();
    DEBUG_PRINT_REGISTER(carryFlag);
    uint16_t result = registers.a + op + carryFlag;
    if(result == 0x00) SetZeroFlag();
    if(result > 0xFF) SetCarryFlag();
    if((registers.a & 0xF) + ((op + carryFlag) & 0xF) > 0xF) SetHalfCarryFlag();
    ClearSubtractFlag();
    registers.a = (uint8_t)result;
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

    SUB(registers.a);
}

void CPU::SUB_B()
{
    DEBUG("0x90 SUB A,B");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register B");
    DEBUG_PRINT_REGISTER(registers.b);

    SUB(registers.b);
}

void CPU::SUB_C()
{
    DEBUG("0x91 SUB A,C");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register C");
    DEBUG_PRINT_REGISTER(registers.c);

    SUB(registers.c);
}

void CPU::SUB_D()
{
    DEBUG("0x92 SUB A,D");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register D");
    DEBUG_PRINT_REGISTER(registers.d);

    SUB(registers.d);
}

void CPU::SUB_E()
{
    DEBUG("0x93 SUB A,E");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register E");
    DEBUG_PRINT_REGISTER(registers.e);

    SUB(registers.e);
}

void CPU::SUB_H()
{
    DEBUG("0x94 SUB A,H");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register H");
    DEBUG_PRINT_REGISTER(registers.h);

    SUB(registers.h);
}

void CPU::SUB_L()
{
    DEBUG("0x94 SUB A,L");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register L");
    DEBUG_PRINT_REGISTER(registers.l);

    SUB(registers.l);
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

    SUB(memory->Read(HL()));
}

void CPU::SUB_Immediate()
{
    DEBUG("0xD6 SUB A,#");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);

    SUB(registers.pc++);
}

void CPU::SUB(uint8_t op)
{
    DEBUG("OPERAND");
    DEBUG_PRINT_REGISTER(op);

    uint16_t result = registers.a - op;

    if(result == 0x0) SetZeroFlag();
    if(result < 0x0) SetCarryFlag();
    if(((registers.a & 0xF) - (op & 0xF)) < 0x0) SetHalfCarryFlag();
    SetSubtractFlag();

    registers.a = (uint8_t)result;

    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG(" ");
}

void CPU::SBC_A()
{
    DEBUG("0x9F SBC A,A");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);

    SBC(registers.a);
}

void CPU::SBC_B()
{
    DEBUG("0x98 SBC A,B");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register B");
    DEBUG_PRINT_REGISTER(registers.b);

    SBC(registers.b);
}

void CPU::SBC_C()
{
    DEBUG("0x99 SBC A,C");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register C");
    DEBUG_PRINT_REGISTER(registers.c);

    SBC(registers.c);
}

void CPU::SBC_D()
{
    DEBUG("0x9A SBC A,D");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register D");
    DEBUG_PRINT_REGISTER(registers.d);

    SBC(registers.d);
}

void CPU::SBC_E()
{
    DEBUG("0x9B SBC A,E");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register E");
    DEBUG_PRINT_REGISTER(registers.e);

    SBC(registers.e);
}

void CPU::SBC_H()
{
    DEBUG("0x9C SBC A,H");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register H");
    DEBUG_PRINT_REGISTER(registers.h);

    SBC(registers.h);
}

void CPU::SBC_L()
{
    DEBUG("0x9D SBC A,L");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register L");
    DEBUG_PRINT_REGISTER(registers.l);

    SBC(registers.l);
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

    SBC(memory->Read(HL()));
}

void CPU::SBC_Immediate()
{
    DEBUG("0xDE SBC A,#");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);

    SBC(registers.pc++);
}

void CPU::SBC(uint8_t op)
{
    DEBUG("CARRY");
    auto carry = GetCarryFlag();
    DEBUG_PRINT_REGISTER(carry);
    DEBUG("OPERAND");
    DEBUG_PRINT_REGISTER(op + carry);

    uint16_t result = registers.a - (op + carry);

    if(result == 0x0) SetZeroFlag();
    if(result < 0x0) SetCarryFlag();

    // TODO : check this
    if((registers.a & 0xF) - (op + carry & 0xF) < 0xF) SetHalfCarryFlag();

    SetSubtractFlag();

    registers.a = (uint8_t)result;


    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG(" ");
}

void CPU::AND_A()
{
    DEBUG("0xA7 AND A,A");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);

    AND(registers.a);
}

void CPU::AND_B()
{
    DEBUG("0xA0 AND A,B");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register B");
    DEBUG_PRINT_REGISTER(registers.b);

    AND(registers.b);
}

void CPU::AND_C()
{
    DEBUG("0xA1 AND A,C");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register C");
    DEBUG_PRINT_REGISTER(registers.c);

    AND(registers.c);
}

void CPU::AND_D()
{
    DEBUG("0xA2 AND A,D");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register D");
    DEBUG_PRINT_REGISTER(registers.d);

    AND(registers.d);
}

void CPU::AND_E()
{
    DEBUG("0xA3 AND A,E");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register E");
    DEBUG_PRINT_REGISTER(registers.e);

    AND(registers.e);
}

void CPU::AND_H()
{
    DEBUG("0xA4 AND A,H");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register H");
    DEBUG_PRINT_REGISTER(registers.h);

    AND(registers.h);
}

void CPU::AND_L()
{
    DEBUG("0xA5 AND A,L");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register L");
    DEBUG_PRINT_REGISTER(registers.l);

    AND(registers.l);
}

void CPU::AND_HL()
{
    DEBUG("0xA6 AND A,HL");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);

    AND(memory->Read(HL()));
}

void CPU::AND_Immediate()
{
    DEBUG("0xE6 AND A,#");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);

    AND(registers.pc++);
}

void CPU::AND(uint8_t op)
{
    uint8_t result = registers.a & op;
    if(result == 0x00) SetZeroFlag();
    ClearSubtractFlag();
    SetHalfCarryFlag();
    ClearCarryFlag();
    registers.a = result;
    DEBUG("Result");
    DEBUG_PRINT_REGISTER(registers.a);
}


void CPU::OR_A()
{
    DEBUG("0xB7 OR A,A");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);

    OR(registers.a);
}

void CPU::OR_B()
{
    DEBUG("0xB0 OR A,B");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register B");
    DEBUG_PRINT_REGISTER(registers.b);

    OR(registers.b);
}

void CPU::OR_C()
{
    DEBUG("0xB1 OR A,C");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register C");
    DEBUG_PRINT_REGISTER(registers.c);

    OR(registers.c);
}

void CPU::OR_D()
{
    DEBUG("0xB2 OR A,D");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register D");
    DEBUG_PRINT_REGISTER(registers.d);

    OR(registers.d);
}

void CPU::OR_E()
{
    DEBUG("0xB3 OR A,E");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register E");
    DEBUG_PRINT_REGISTER(registers.e);

    OR(registers.e);
}

void CPU::OR_H()
{
    DEBUG("0xB4 OR A,H");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register H");
    DEBUG_PRINT_REGISTER(registers.h);

    OR(registers.h);
}

void CPU::OR_L()
{
    DEBUG("0xB5 OR A,L");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);
    DEBUG("Register L");
    DEBUG_PRINT_REGISTER(registers.l);

    OR(registers.l);
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

    OR(memory->Read(HL()));
}

void CPU::OR_Immediate()
{
    DEBUG("0xF6 OR A,#");
    DEBUG("Register A");
    DEBUG_PRINT_REGISTER(registers.a);

    OR(registers.pc++);
}

void CPU::OR(uint8_t op)
{
    DEBUG("Operand");
    DEBUG_PRINT_REGISTER(op);
    uint8_t result = registers.a | op;
    if(result == 0x00) SetZeroFlag();
    ClearSubtractFlag();
    ClearHalfCarryFlag();
    ClearCarryFlag();
    registers.a = result;

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

void CPU::INC_VALUE_AT_HL()
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

void CPU::DEC_VALUE_AT_HL()
{
    uint8_t v = memory->Read(HL());
    DEC(v);
    memory->Write(HL(), v);
}

void CPU::DEC(uint8_t& reg)
{
    uint8_t tReg = reg - 0x1;

    // TODO : verify that this is correct
    if(((tReg & 0xF) + (reg & 0xF)) & 0x10) SetHalfCarryFlag();
    reg = tReg;
    if(reg == 0x00) SetZeroFlag();
    SetSubtractFlag();
}

void CPU::ADD_BC_TO_HL()
{
    ADD_TO_HL(BC());
}

void CPU::ADD_DE_TO_HL()
{
    ADD_TO_HL(DE());
}

void CPU::ADD_HL_TO_HL()
{
    ADD_TO_HL(HL());
}

void CPU::ADD_SP_TO_HL()
{
    ADD_TO_HL(registers.sp);
}

void CPU::ADD_TO_HL(uint16_t op)
{
    uint16_t hl = HL();
    uint32_t result = hl + op;
    ClearSubtractFlag();
    if(result > 0xFFFF) SetCarryFlag();
    if((hl & 0xFFF) + (op & 0xFFF) > 0xFFF) SetHalfCarryFlag();
    SetHL((uint16_t)result);
}

void CPU::ADD_SP()
{
    uint8_t op = registers.pc++;
    registers.sp += op;

    ClearZeroFlag();
    ClearSubtractFlag();

    // TODO : what to do with H and C flags?
}

void CPU::INC_BC()
{
    SetBC(BC() + 0x1);
}

void CPU::INC_DE()
{
    SetDE(DE() + 0x1);
}

void CPU::INC_HL()
{
    SetHL(HL() + 0x1);
}

void CPU::INC_SP()
{
    SetSP(registers.sp + 0x1);
}

void CPU::DEC_BC()
{
    SetBC(BC() - 0x1);
}

void CPU::INC_DE()
{
    SetDE(DE() - 0x1);
}

void CPU::DEC_HL()
{
    SetHL(HL() - 0x1);
}

void CPU::DEC_SP()
{
    SetSP(registers.sp - 0x1);
}

void CPU::SWAP_A()
{
    SWAP(registers.a);
}

void CPU::SWAP_B()
{
    SWAP(registers.b);
}

void CPU::SWAP_C()
{
    SWAP(registers.c);
}

void CPU::SWAP_D()
{
    SWAP(registers.d);
}

void CPU::SWAP_E()
{
    SWAP(registers.e);
}

void CPU::SWAP_H()
{
    SWAP(registers.h);
}

void CPU::SWAP_L()
{
    SWAP(registers.l);
}

void CPU::SWAP_VALUE_AT_HL()
{
    auto val = memory->Read(HL());
    val = ((val & 0xF) << 4 | (val & 0xF0) >> 4);
    if(val == 0x00) SetZeroFlag();
    memory->Write(HL(), val);
    ClearSubtractFlag();
    ClearHalfCarryFlag();
    ClearCarryFlag();
}

void CPU::SWAP(uint8_t& op)
{
    op = ((op & 0xF) << 4 | (op & 0xF0) >> 4);
    if(op == 0x00) SetZeroFlag();
    ClearSubtractFlag();
    ClearHalfCarryFlag();
    ClearCarryFlag();
}

void CPU::DAA()
{
    if(!IsSubtractFlagSet())
    {
        if(IsCarryFlagSet() || registers.a > 0x99)
        {
            registers.a += 0x60;
            SetCarryFlag();
        }
        if(IsHalfCarryFlagSet() | (registers.a & 0x0F) > 0x09)
        {
            registers.a += 0x6;
        }
    }
    else
    {
        if(IsCarryFlagSet()) registers.a -= 0x60;
        if(IsHalfCarryFlagSet()) registers.a -= 0x6;
    }

    if(registers.a == 0x0) SetZeroFlag();
    ClearHalfCarryFlag();
}

void CPU::CPL()
{
    registers.a = (uint8_t)~registers.a;
    SetSubtractFlag();
    SetHalfCarryFlag();
}

void CPU::CCF()
{
    if(IsCarryFlagSet()) ClearCarryFlag();
    else SetCarryFlag();

    ClearSubtractFlag();
    ClearHalfCarryFlag();
}

void CPU::SCF()
{
    SetCarryFlag();
    ClearHalfCarryFlag();
    ClearSubtractFlag();
}

void CPU::HALT()
{
    // 0x76
    // TODO : eh?
}

void CPU::STOP()
{
    // 0x10 0x00
    // TODO : eh?
}

void CPU::DI()
{
    interruptState = InterruptState::NeedToDisable;
}

void CPU::EI()
{
    interruptState = InterruptState::NeedToEnable;
}

void CPU::RLCA()
{
    uint8_t leftbit = registers.a & 0b10000000;
    leftbit >> 7;
    if(leftbit == 0x1) SetCarryFlag();
    else ClearCarryFlag();
    registers.a << 1;
    registers.a |= leftbit;

    if(registers.a == 0x0) SetZeroFlag();
    ClearSubtractFlag();
    ClearHalfCarryFlag();
}

void CPU::RLA()
{
    uint8_t leftbit = registers.a & 0b10000000;
    leftbit >> 7;

    uint8_t carry = GetCarryFlag();
    carry >> 4;

    if(leftbit == 0x0) ClearCarryFlag();
    else SetCarryFlag();

    registers.a << 1;
    registers.a |= carry;

    if(registers.a == 0x0) SetZeroFlag();
    ClearSubtractFlag();
    ClearHalfCarryFlag();
}

void CPU::RRCA()
{
    uint8_t rightbit = registers.a & 0b00000001;
    if(rightbit == 0x1) SetCarryFlag();
    else ClearCarryFlag();
    registers.a >> 1;
    registers.a |= rightbit;

    if(registers.a == 0x0) SetZeroFlag();
    ClearSubtractFlag();
    ClearHalfCarryFlag();
}

void CPU::RRA()
{
    uint8_t rightbit = registers.a & 0b00000001;

    uint8_t carry = GetCarryFlag();
    carry >> 4;

    if(rightbit == 0x0) ClearCarryFlag();
    else SetCarryFlag();

    registers.a >> 1;
    registers.a |= carry;

    if(registers.a == 0x0) SetZeroFlag();
    ClearSubtractFlag();
    ClearHalfCarryFlag();
}

void CPU::RLC_A()
{
    RLC_n(registers.a);
}

void CPU::RLC_B()
{
    RLC_n(registers.b);
}

void CPU::RLC_C()
{
    RLC_n(registers.c);
}

void CPU::RLC_D()
{
    RLC_n(registers.d);
}

void CPU::RLC_E()
{
    RLC_n(registers.e);
}

void CPU::RLC_H()
{
    RLC_n(registers.h);
}

void CPU::RLC_L()
{
    RLC_n(registers.l);
}

void CPU::RLC_VALUE_AT_HL()
{
    auto val = memory->Read(HL());
    uint8_t leftbit = val & 0b10000000;
    leftbit >> 7;
    if(leftbit == 0x0) ClearCarryFlag();
    else SetCarryFlag();

    val << 1;
    val |= leftbit;

    if(val == 0x0) SetZeroFlag();
    ClearSubtractFlag();
    ClearHalfCarryFlag();    
}

void CPU::RLC_n(uint8_t& op)
{
    uint8_t leftbit = op & 0b10000000;
    leftbit >> 7;
    if(leftbit == 0x0) ClearCarryFlag();
    else SetCarryFlag();

    op << 1;
    op |= leftbit;    

    if(op == 0x0) SetZeroFlag();
    ClearSubtractFlag();
    ClearHalfCarryFlag();
}

void CPU::RL_A()
{
    RL_n(registers.a);
}

void CPU::RL_B()
{
    RL_n(registers.b);
}

void CPU::RL_C()
{
    RL_n(registers.c);
}

void CPU::RL_D()
{
    RL_n(registers.d);
}

void CPU::RL_E()
{
    RL_n(registers.e);
}

void CPU::RL_H()
{
    RL_n(registers.h);
}

void CPU::RL_L()
{
    RL_n(registers.l);
}

void CPU::RL_VALUE_AT_HL()
{
    auto val = memory->Read(HL());
    uint8_t leftbit = val & 0b10000000;
    leftbit >>= 7;

    uint8_t carry = 0x0;
    if(IsCarryFlagSet()) carry = 0x1;

    if(leftbit == 0x0) ClearCarryFlag();
    else SetCarryFlag();

    val <<= 1;
    val |= carry;

    if(val == 0x0) SetZeroFlag();
    ClearSubtractFlag();
    ClearHalfCarryFlag();

    memory->Write(HL(), val);
}

void CPU::RL_n(uint8_t& op)
{
    uint8_t leftbit = op & 0b10000000;
    leftbit >>= 7;

    uint8_t carry = 0x0;

    if(IsCarryFlagSet()) carry = 0x1; 

    if(leftbit == 0x0) ClearCarryFlag();
    else SetCarryFlag();

    op <<= 1;
    op |= carry;

    if(op == 0x0) SetZeroFlag();
    ClearSubtractFlag();
    ClearHalfCarryFlag();
}

void CPU::RR_A()
{
    RR_n(registers.a);
}

void CPU::RR_B()
{
    RR_n(registers.b);
}

void CPU::RR_C()
{
    RR_n(registers.c);
}

void CPU::RR_D()
{
    RR_n(registers.d);
}

void CPU::RR_E()
{
    RR_n(registers.e);
}

void CPU::RR_H()
{
    RR_n(registers.h);
}

void CPU::RR_L()
{
    RR_n(registers.l);
}

void CPU::RR_VALUE_AT_HL()
{
    auto val = memory->Read(HL());
    uint8_t rightbit = val & 0b00000001;

    uint8_t carry = 0x0;

    if(IsCarryFlagSet()) carry = 0x1;

    if(rightbit == 0x0) ClearCarryFlag();
    else SetCarryFlag();

    val >>= 1;
    carry <<= 7;
    val |= carry;

    if(val == 0x0) SetZeroFlag();
    ClearSubtractFlag();
    ClearHalfCarryFlag();

    memory->Write(HL(), val);
}

void CPU::RR_n(uint8_t& op)
{
    uint8_t rightbit = op & 0b00000001;

    uint8_t carry = 0x0;

    if(IsCarryFlagSet()) carry = 0x1;

    if(rightbit == 0x0) ClearCarryFlag();
    else SetCarryFlag();

    op >>= 1;
    carry <<= 7;
    op |= carry;

    if(op == 0x0) SetZeroFlag();
    ClearSubtractFlag();
    ClearHalfCarryFlag();
}

void CPU::SLA_A()
{
    SLA_n(registers.a);
}

void CPU::SLA_B()
{
    SLA_n(registers.b);
}

void CPU::SLA_C()
{
    SLA_n(registers.c);
}

void CPU::SLA_D()
{
    SLA_n(registers.d);
}

void CPU::SLA_E()
{
    SLA_n(registers.e);
}

void CPU::SLA_H()
{
    SLA_n(registers.h);
}

void CPU::SLA_L()
{
    SLA_n(registers.l);
}

void CPU::SLA_VALUE_AT_HL()
{
    auto val = memory->Read(HL());
    auto leftbit = val & 0b10000000;
    leftbit >>= 7;
    if(leftbit == 0x0) ClearCarryFlag();
    else SetCarryFlag();
    val <<= 1;
    val &= 0b11111110;

    ClearHalfCarryFlag();
    ClearSubtractFlag();
    if(val == 0x0) SetZeroFlag();

    memory->Write(HL(), val);
}

void CPU::SLA_n(uint8_t& op)
{
    auto leftbit = op & 0b10000000;
    leftbit >>= 7;
    if(leftbit == 0x0) ClearCarryFlag();
    else SetCarryFlag();
    op <<= 1;
    op &= 0b11111110;

    ClearHalfCarryFlag();
    ClearSubtractFlag();
    if(op == 0x0) SetZeroFlag();
}

void CPU::SRA_A()
{
    SRA_n(registers.a);
}

void CPU::SRA_B()
{
    SRA_n(registers.b);
}

void CPU::SRA_C()
{
    SRA_n(registers.c);
}

void CPU::SRA_D()
{
    SRA_n(registers.d);
}

void CPU::SRA_E()
{
    SRA_n(registers.e);
}

void CPU::SRA_H()
{
    SRA_n(registers.h);
}

void CPU::SRA_L()
{
    SRA_n(registers.l);
}

void CPU::SRA_VALUE_AT_HL()
{
    auto val = memory->Read(HL());
    auto leftbit = val & 0b10000000;
    auto rightbit = val & 0b00000001;

    if(rightbit == 0x0) ClearCarryFlag();
    else SetCarryFlag();

    val >>= 1;
    val |= leftbit;

    memory->Write(HL(), val);
}

void CPU::SRA_n(uint8_t& op)
{
    auto leftbit = op & 0b10000000;
    auto rightbit = op & 0b00000001;

    if(rightbit == 0x0) ClearCarryFlag();
    else SetCarryFlag();

    op >>= 1;
    op |= leftbit;

    if(op == 0x0) SetZeroFlag();

    ClearHalfCarryFlag();
    ClearSubtractFlag();
}

void CPU::SRL_A()
{
    SRL_n(registers.a);
}

void CPU::SRL_B()
{
    SRL_n(registers.b);
}

void CPU::SRL_C()
{
    SRL_n(registers.c);
}

void CPU::SRL_D()
{
    SRL_n(registers.e);
}

void CPU::SRL_E()
{
    SRL_n(registers.e);
}

void CPU::SRL_H()
{
    SRL_n(registers.h);
}

void CPU::SRL_L()
{
    SRL_n(registers.l);
}

void CPU::SRL_VALUE_AT_HL()
{
    auto val = memory->Read(HL());
    auto rightbit = val & 0b00000001;

    if(rightbit == 0x0) ClearCarryFlag();
    else SetCarryFlag();

    val >>= 1;
    val &= 0b01111111;

    if(val == 0x0) SetZeroFlag();

    ClearHalfCarryFlag();
    ClearSubtractFlag();

    memory->Write(HL(), val);
}

void CPU::SRL_n(uint8_t& op)
{
    auto rightbit = op & 0b00000001;

    if(rightbit == 0x0) ClearCarryFlag();
    else SetCarryFlag();

    op >>= 1;
    op &= 0b01111111;

    if(op == 0x0) SetZeroFlag();

    ClearHalfCarryFlag();
    ClearSubtractFlag();
}

void CPU::BIT_0_A()
{
    BIT_b_N(0, registers.a);
}

void CPU::BIT_0_B()
{
    BIT_b_N(0, registers.b);
}

void CPU::BIT_0_C()
{
    BIT_b_N(0, registers.c);
}

void CPU::BIT_0_D()
{
    BIT_b_N(0, registers.d);
}

void CPU::BIT_0_E()
{
    BIT_b_N(0, registers.e);
}

void CPU::BIT_0_H()
{
    BIT_b_N(0, registers.h);
}

void CPU::BIT_0_L()
{
    BIT_b_N(0, registers.l);
}

void CPU::BIT_0_VALUE_AT_HL()
{
    auto op = memory->Read(HL());
    BIT_b_N(0, op);
}

void CPU::BIT_1_A()
{
    BIT_b_N(1, registers.a);
}

void CPU::BIT_1_B()
{
    BIT_b_N(1, registers.b);
}

void CPU::BIT_1_C()
{
    BIT_b_N(1, registers.c);
}

void CPU::BIT_1_D()
{
    BIT_b_N(1, registers.d);
}

void CPU::BIT_1_E()
{
    BIT_b_N(1, registers.e);
}

void CPU::BIT_1_H()
{
    BIT_b_N(1, registers.h);
}

void CPU::BIT_1_L()
{
    BIT_b_N(1, registers.l);
}

void CPU::BIT_1_VALUE_AT_HL()
{
    auto op = memory->Read(HL());
    BIT_b_N(1, op);
}

void CPU::BIT_2_A()
{
    BIT_b_N(2, registers.a);
}

void CPU::BIT_2_B()
{
    BIT_b_N(2, registers.b);
}

void CPU::BIT_2_C()
{
    BIT_b_N(2, registers.c);
}

void CPU::BIT_2_D()
{
    BIT_b_N(2, registers.d);
}

void CPU::BIT_2_E()
{
    BIT_b_N(2, registers.e);
}

void CPU::BIT_2_H()
{
    BIT_b_N(2, registers.h);
}

void CPU::BIT_2_L()
{
    BIT_b_N(2, registers.l);
}

void CPU::BIT_2_VALUE_AT_HL()
{
    auto op = memory->Read(HL());
    BIT_b_N(2, op);
}

void CPU::BIT_3_A()
{
    BIT_b_N(3, registers.a);
}

void CPU::BIT_3_B()
{
    BIT_b_N(3, registers.b);
}

void CPU::BIT_3_C()
{
    BIT_b_N(3, registers.c);
}

void CPU::BIT_3_D()
{
    BIT_b_N(3, registers.d);
}

void CPU::BIT_3_E()
{
    BIT_b_N(3, registers.e);
}

void CPU::BIT_3_H()
{
    BIT_b_N(3, registers.h);
}

void CPU::BIT_3_L()
{
    BIT_b_N(3, registers.l);
}

void CPU::BIT_3_VALUE_AT_HL()
{
    auto op = memory->Read(HL());
    BIT_b_N(3, op);
}

void CPU::BIT_4_A()
{
    BIT_b_N(4, registers.a);
}

void CPU::BIT_4_B()
{
    BIT_b_N(4, registers.b);
}

void CPU::BIT_4_C()
{
    BIT_b_N(4, registers.c);
}

void CPU::BIT_4_D()
{
    BIT_b_N(4, registers.d);
}

void CPU::BIT_4_E()
{
    BIT_b_N(4, registers.e);
}

void CPU::BIT_4_H()
{
    BIT_b_N(4, registers.h);
}

void CPU::BIT_4_L()
{
    BIT_b_N(4, registers.l);
}

void CPU::BIT_4_VALUE_AT_HL()
{
    auto op = memory->Read(HL());
    BIT_b_N(4, op);
}

void CPU::BIT_5_A()
{
    BIT_b_N(5, registers.a);
}

void CPU::BIT_5_B()
{
    BIT_b_N(5, registers.b);
}

void CPU::BIT_5_C()
{
    BIT_b_N(5, registers.c);
}

void CPU::BIT_5_D()
{
    BIT_b_N(5, registers.d);
}

void CPU::BIT_5_E()
{
    BIT_b_N(5, registers.e);
}

void CPU::BIT_5_H()
{
    BIT_b_N(5, registers.h);
}

void CPU::BIT_5_L()
{
    BIT_b_N(5, registers.l);
}

void CPU::BIT_5_VALUE_AT_HL()
{
    auto op = memory->Read(HL());
    BIT_b_N(5, op);
}

void CPU::BIT_6_A()
{
    BIT_b_N(6, registers.a);
}

void CPU::BIT_6_B()
{
    BIT_b_N(6, registers.b);
}

void CPU::BIT_6_C()
{
    BIT_b_N(6, registers.c);
}

void CPU::BIT_6_D()
{
    BIT_b_N(6, registers.d);
}

void CPU::BIT_6_E()
{
    BIT_b_N(6, registers.e);
}

void CPU::BIT_6_H()
{
    BIT_b_N(6, registers.h);
}

void CPU::BIT_6_L()
{
    BIT_b_N(6, registers.l);
}

void CPU::BIT_6_VALUE_AT_HL()
{
    auto op = memory->Read(HL());
    BIT_b_N(6, op);
}

void CPU::BIT_7_A()
{
    BIT_b_N(7, registers.a);
}

void CPU::BIT_7_B()
{
    BIT_b_N(7, registers.b);
}

void CPU::BIT_7_C()
{
    BIT_b_N(7, registers.c);
}

void CPU::BIT_7_D()
{
    BIT_b_N(7, registers.d);
}

void CPU::BIT_7_E()
{
    BIT_b_N(7, registers.e);
}

void CPU::BIT_7_H()
{
    BIT_b_N(7, registers.h);
}

void CPU::BIT_7_L()
{
    BIT_b_N(7, registers.l);
}

void CPU::BIT_7_VALUE_AT_HL()
{
    auto op = memory->Read(HL());
    BIT_b_N(7, op);
}

void CPU::BIT_b_N(uint8_t bitPos, uint8_t &op)
{
    assert(bitPos <= 7);
    auto s = op >> bitPos;
    if((s & 0b00000001) == 0b00000000) SetZeroFlag();
    SetHalfCarryFlag();
    ClearSubtractFlag();
}

void CPU::SET_0_A()
{
    SET_b_N(0, registers.a);
}

void CPU::SET_0_B()
{
    SET_b_N(0, registers.b);
}

void CPU::SET_0_C()
{
    SET_b_N(0, registers.c);
}

void CPU::SET_0_D()
{
    SET_b_N(0, registers.d);
}

void CPU::SET_0_E()
{
    SET_b_N(0, registers.e);
}

void CPU::SET_0_H()
{
    SET_b_N(0, registers.h);
}

void CPU::SET_0_L()
{
    SET_b_N(0, registers.l);
}

void CPU::SET_0_VALUE_AT_HL()
{
    auto val = memory->Read((HL()));
    val |= 0b00000001;
    memory->Write(HL(), val);
}

void CPU::SET_1_A()
{
    SET_b_N(1, registers.a);
}

void CPU::SET_1_B()
{
    SET_b_N(1, registers.b);
}

void CPU::SET_1_C()
{
    SET_b_N(1, registers.c);
}

void CPU::SET_1_D()
{
    SET_b_N(1, registers.d);
}

void CPU::SET_1_E()
{
    SET_b_N(1, registers.e);
}

void CPU::SET_1_H()
{
    SET_b_N(1, registers.h);
}

void CPU::SET_1_L()
{
    SET_b_N(1, registers.l);
}

void CPU::SET_1_VALUE_AT_HL()
{
    auto val = memory->Read((HL()));
    val |= 0b00000010;
    memory->Write(HL(), val);
}

void CPU::SET_2_A()
{
    SET_b_N(2, registers.a);
}

void CPU::SET_2_B()
{
    SET_b_N(2, registers.b);
}

void CPU::SET_2_C()
{
    SET_b_N(2, registers.c);
}

void CPU::SET_2_D()
{
    SET_b_N(2, registers.d);
}

void CPU::SET_2_E()
{
    SET_b_N(2, registers.e);
}

void CPU::SET_2_H()
{
    SET_b_N(2, registers.h);
}

void CPU::SET_2_L()
{
    SET_b_N(2, registers.l);
}

void CPU::SET_2_VALUE_AT_HL()
{
    auto val = memory->Read((HL()));
    val |= 0b00000100;
    memory->Write(HL(), val);
}

void CPU::SET_3_A()
{
    SET_b_N(3, registers.a);
}

void CPU::SET_3_B()
{
    SET_b_N(3, registers.b);
}

void CPU::SET_3_C()
{
    SET_b_N(3, registers.c);
}

void CPU::SET_3_D()
{
    SET_b_N(3, registers.d);
}

void CPU::SET_3_E()
{
    SET_b_N(3, registers.e);
}

void CPU::SET_3_H()
{
    SET_b_N(3, registers.h);
}

void CPU::SET_3_L()
{
    SET_b_N(3, registers.l);
}

void CPU::SET_3_VALUE_AT_HL()
{
    auto val = memory->Read((HL()));
    val |= 0b00001000;
    memory->Write(HL(), val);
}

void CPU::SET_4_A()
{
    SET_b_N(4, registers.a);
}

void CPU::SET_4_B()
{
    SET_b_N(4, registers.b);
}

void CPU::SET_4_C()
{
    SET_b_N(4, registers.c);
}

void CPU::SET_4_D()
{
    SET_b_N(4, registers.d);
}

void CPU::SET_4_E()
{
    SET_b_N(4, registers.e);
}

void CPU::SET_4_H()
{
    SET_b_N(4, registers.h);
}

void CPU::SET_4_L()
{
    SET_b_N(4, registers.l);
}

void CPU::SET_4_VALUE_AT_HL()
{
    auto val = memory->Read((HL()));
    val |= 0b00010000;
    memory->Write(HL(), val);
}

void CPU::SET_5_A()
{
    SET_b_N(5, registers.a);
}

void CPU::SET_5_B()
{
    SET_b_N(5, registers.b);
}

void CPU::SET_5_C()
{
    SET_b_N(5, registers.c);
}

void CPU::SET_5_D()
{
    SET_b_N(5, registers.d);
}

void CPU::SET_5_E()
{
    SET_b_N(5, registers.e);
}

void CPU::SET_5_H()
{
    SET_b_N(5, registers.h);
}

void CPU::SET_5_L()
{
    SET_b_N(5, registers.l);
}

void CPU::SET_5_VALUE_AT_HL()
{
    auto val = memory->Read((HL()));
    val |= 0b00100000;
    memory->Write(HL(), val);
}

void CPU::SET_6_A()
{
    SET_b_N(6, registers.a);
}

void CPU::SET_6_B()
{
    SET_b_N(6, registers.b);
}

void CPU::SET_6_C()
{
    SET_b_N(6, registers.c);
}

void CPU::SET_6_D()
{
    SET_b_N(6, registers.d);
}

void CPU::SET_6_E()
{
    SET_b_N(6, registers.e);
}

void CPU::SET_6_H()
{
    SET_b_N(6, registers.h);
}

void CPU::SET_6_L()
{
    SET_b_N(6, registers.l);
}

void CPU::SET_6_VALUE_AT_HL()
{
    auto val = memory->Read((HL()));
    val |= 0b01000000;
    memory->Write(HL(), val);
}

void CPU::SET_7_A()
{
    SET_b_N(7, registers.a);
}

void CPU::SET_7_B()
{
    SET_b_N(7, registers.b);
}

void CPU::SET_7_C()
{
    SET_b_N(7, registers.c);
}

void CPU::SET_7_D()
{
    SET_b_N(7, registers.d);
}

void CPU::SET_7_E()
{
    SET_b_N(7, registers.e);
}

void CPU::SET_7_H()
{
    SET_b_N(7, registers.h);
}

void CPU::SET_7_L()
{
    SET_b_N(7, registers.l);
}

void CPU::SET_7_VALUE_AT_HL()
{
    auto val = memory->Read((HL()));
    val |= 0b10000000;
    memory->Write(HL(), val);
}

void CPU::SET_b_N(uint8_t bitPos, uint8_t &op)
{
    assert(bitPos >= 0 && bitPos <= 7);
    if(bitPos == 0) op |= 0b00000001;
    else if(bitPos == 1) op |= 0b00000010;
    else if(bitPos == 2) op |= 0b00000100;
    else if(bitPos == 3) op |= 0b00001000;
    else if(bitPos == 4) op |= 0b00010000;
    else if(bitPos == 5) op |= 0b00100000;
    else if(bitPos == 6) op |= 0b01000000;
    else if(bitPos == 7) op |= 0b10000000;
}

void CPU::RES_0_A()
{
    RES_b_N(0, registers.a);
}

void CPU::RES_0_B()
{
    RES_b_N(0, registers.b);
}

void CPU::RES_0_C()
{
    RES_b_N(0, registers.c);
}

void CPU::RES_0_D()
{
    RES_b_N(0, registers.d);
}

void CPU::RES_0_E()
{
    RES_b_N(0, registers.e);
}

void CPU::RES_0_H()
{
    RES_b_N(0, registers.h);
}

void CPU::RES_0_L()
{
    RES_b_N(0, registers.l);
}

void CPU::RES_0_VALUE_AT_HL()
{
    auto val = memory->Read((HL()));
    val &= 0b11111110;
    memory->Write(HL(), val);
}

void CPU::RES_1_A()
{
    RES_b_N(1, registers.a);
}

void CPU::RES_1_B()
{
    RES_b_N(1, registers.b);
}

void CPU::RES_1_C()
{
    RES_b_N(1, registers.c);
}

void CPU::RES_1_D()
{
    RES_b_N(1, registers.d);
}

void CPU::RES_1_E()
{
    RES_b_N(1, registers.e);
}

void CPU::RES_1_H()
{
    RES_b_N(1, registers.h);
}

void CPU::RES_1_L()
{
    RES_b_N(1, registers.l);
}

void CPU::RES_1_VALUE_AT_HL()
{
    auto val = memory->Read((HL()));
    val &= 0b11111101;
    memory->Write(HL(), val);
}

void CPU::RES_2_A()
{
    RES_b_N(2, registers.a);
}

void CPU::RES_2_B()
{
    RES_b_N(2, registers.b);
}

void CPU::RES_2_C()
{
    RES_b_N(2, registers.c);
}

void CPU::RES_2_D()
{
    RES_b_N(2, registers.d);
}

void CPU::RES_2_E()
{
    RES_b_N(2, registers.e);
}

void CPU::RES_2_H()
{
    RES_b_N(2, registers.h);
}

void CPU::RES_2_L()
{
    RES_b_N(2, registers.l);
}

void CPU::RES_2_VALUE_AT_HL()
{
    auto val = memory->Read((HL()));
    val &= 0b11111011;
    memory->Write(HL(), val);
}

void CPU::RES_3_A()
{
    RES_b_N(3, registers.a);
}

void CPU::RES_3_B()
{
    RES_b_N(3, registers.b);
}

void CPU::RES_3_C()
{
    RES_b_N(3, registers.c);
}

void CPU::RES_3_D()
{
    RES_b_N(3, registers.d);
}

void CPU::RES_3_E()
{
    RES_b_N(3, registers.e);
}

void CPU::RES_3_H()
{
    RES_b_N(3, registers.h);
}

void CPU::RES_3_L()
{
    RES_b_N(3, registers.l);
}

void CPU::RES_3_VALUE_AT_HL()
{
    auto val = memory->Read((HL()));
    val &= 0b11110111;
    memory->Write(HL(), val);
}

void CPU::RES_4_A()
{
    RES_b_N(4, registers.a);
}

void CPU::RES_4_B()
{
    RES_b_N(4, registers.b);
}

void CPU::RES_4_C()
{
    RES_b_N(4, registers.c);
}

void CPU::RES_4_D()
{
    RES_b_N(4, registers.d);
}

void CPU::RES_4_E()
{
    RES_b_N(4, registers.e);
}

void CPU::RES_4_H()
{
    RES_b_N(4, registers.h);
}

void CPU::RES_4_L()
{
    RES_b_N(4, registers.l);
}

void CPU::RES_4_VALUE_AT_HL()
{
    auto val = memory->Read((HL()));
    val &= 0b11101111;
    memory->Write(HL(), val);
}

void CPU::RES_5_A()
{
    RES_b_N(5, registers.a);
}

void CPU::RES_5_B()
{
    RES_b_N(5, registers.b);
}

void CPU::RES_5_C()
{
    RES_b_N(5, registers.c);
}

void CPU::RES_5_D()
{
    RES_b_N(5, registers.d);
}

void CPU::RES_5_E()
{
    RES_b_N(5, registers.e);
}

void CPU::RES_5_H()
{
    RES_b_N(5, registers.h);
}

void CPU::RES_5_L()
{
    RES_b_N(5, registers.l);
}

void CPU::RES_5_VALUE_AT_HL()
{
    auto val = memory->Read((HL()));
    val &= 0b11011111;
    memory->Write(HL(), val);
}

void CPU::RES_6_A()
{
    RES_b_N(6, registers.a);
}

void CPU::RES_6_B()
{
    RES_b_N(6, registers.b);
}

void CPU::RES_6_C()
{
    RES_b_N(6, registers.c);
}

void CPU::RES_6_D()
{
    RES_b_N(6, registers.d);
}

void CPU::RES_6_E()
{
    RES_b_N(6, registers.e);
}

void CPU::RES_6_H()
{
    RES_b_N(6, registers.h);
}

void CPU::RES_6_L()
{
    RES_b_N(6, registers.l);
}

void CPU::RES_6_VALUE_AT_HL()
{
    auto val = memory->Read((HL()));
    val &= 0b10111111;
    memory->Write(HL(), val);
}

void CPU::RES_7_A()
{
    RES_b_N(7, registers.a);
}

void CPU::RES_7_B()
{
    RES_b_N(7, registers.b);
}

void CPU::RES_7_C()
{
    RES_b_N(7, registers.c);
}

void CPU::RES_7_D()
{
    RES_b_N(7, registers.d);
}

void CPU::RES_7_E()
{
    RES_b_N(7, registers.e);
}

void CPU::RES_7_H()
{
    RES_b_N(7, registers.h);
}

void CPU::RES_7_L()
{
    RES_b_N(7, registers.l);
}

void CPU::RES_7_VALUE_AT_HL()
{
    auto val = memory->Read((HL()));
    val &= 0b01111111;
    memory->Write(HL(), val);
}

void CPU::RES_b_N(uint8_t bitPos, uint8_t &op)
{
    assert(bitPos >= 0 && bitPos <= 7);
    if(bitPos == 0) op &= 0b11111110;
    else if(bitPos == 1) op &= 0b11111101;
    else if(bitPos == 2) op &= 0b11111011;
    else if(bitPos == 3) op &= 0b11110111;
    else if(bitPos == 4) op &= 0b11101111;
    else if(bitPos == 5) op &= 0b11011111;
    else if(bitPos == 6) op &= 0b10111111;
    else if(bitPos == 7) op &= 0b01111111;
}

void CPU::JP_nn()
{
    auto lsb = registers.pc++;
    auto msb = registers.pc++;

    auto addr = (uint16_t) (msb << 8 | lsb);
    registers.pc = addr;
}

void CPU::JP_NZ_nn()
{
    if(!IsZeroFlagSet())
    {
        auto lsb = registers.pc++;
        auto msb = registers.pc++;

        auto addr = (uint16_t) (msb << 8 | lsb);
        registers.pc = addr;
    }
}

void CPU::JP_Z_nn()
{
    if(IsZeroFlagSet())
    {
        auto lsb = registers.pc++;
        auto msb = registers.pc++;

        auto addr = (uint16_t) (msb << 8 | lsb);
        registers.pc = addr;
    }
}

void CPU::JP_NC_nn()
{
    if(!IsCarryFlagSet())
    {
        auto lsb = registers.pc++;
        auto msb = registers.pc++;

        auto addr = (uint16_t) (msb << 8 | lsb);
        registers.pc = addr;
    }
}

void CPU::JP_C_nn()
{
    if(IsCarryFlagSet())
    {
        auto lsb = registers.pc++;
        auto msb = registers.pc++;

        auto addr = (uint16_t) (msb << 8 | lsb);
        registers.pc = addr;
    }
}

void CPU::JP_HL()
{
    registers.pc = HL();
}

void CPU::JR_n()
{
    int8_t n = registers.pc++;
    registers.pc += n;
}

void CPU::JR_NZ_n()
{
    if(!IsZeroFlagSet())
    {
        int8_t n = registers.pc++;
        registers.pc += n;
    }
}

void CPU::JR_Z_n()
{
    if(IsZeroFlagSet())
    {
        int8_t n = registers.pc++;
        registers.pc += n;
    }
}

void CPU::JR_NC_n()
{
    if(!IsCarryFlagSet())
    {
        int8_t n = registers.pc++;
        registers.pc += n;
    }
}

void CPU::JR_C_n()
{
    if(IsCarryFlagSet())
    {
        int8_t n = registers.pc++;
        registers.pc += n;
    }
}

void CPU::CALL_nn()
{
    auto lsb = registers.pc++;
    auto msb = registers.pc++;

    memory->Write(registers.sp--, msb);
    memory->Write(registers.sp--, lsb);
}

void CPU::CALL_NZ_nn()
{
    if(!IsZeroFlagSet())
    {
        auto lsb = registers.pc++;
        auto msb = registers.pc++;

        memory->Write(registers.sp--, msb);
        memory->Write(registers.sp--, lsb);
    }
}

void CPU::CALL_Z_nn()
{
    if(IsZeroFlagSet())
    {
        auto lsb = registers.pc++;
        auto msb = registers.pc++;

        memory->Write(registers.sp--, msb);
        memory->Write(registers.sp--, lsb);
    }
}

void CPU::CALL_NC_nn()
{
    if(!IsCarryFlagSet())
    {
        auto lsb = registers.pc++;
        auto msb = registers.pc++;

        memory->Write(registers.sp--, msb);
        memory->Write(registers.sp--, lsb);
    }
}

void CPU::CALL_C_nn()
{
    if(IsCarryFlagSet())
    {
        auto lsb = registers.pc++;
        auto msb = registers.pc++;

        memory->Write(registers.sp--, msb);
        memory->Write(registers.sp--, lsb);
    }
}

void CPU::RST_00H()
{
    uint8_t msb = (uint8_t) registers.pc >> 8;
    uint8_t lsb = (uint8_t) registers.pc;

    memory->Write(registers.sp--, msb);
    memory->Write(registers.sp--, lsb);

    registers.pc = 0x0000;
}

void CPU::RST_08H()
{
    uint8_t msb = (uint8_t) registers.pc >> 8;
    uint8_t lsb = (uint8_t) registers.pc;

    memory->Write(registers.sp--, msb);
    memory->Write(registers.sp--, lsb);

    registers.pc = 0x0008;
}

void CPU::RST_10H()
{
    uint8_t msb = (uint8_t) registers.pc >> 8;
    uint8_t lsb = (uint8_t) registers.pc;

    memory->Write(registers.sp--, msb);
    memory->Write(registers.sp--, lsb);

    registers.pc = 0x0010;
}

void CPU::RST_18H()
{
    uint8_t msb = (uint8_t) registers.pc >> 8;
    uint8_t lsb = (uint8_t) registers.pc;

    memory->Write(registers.sp--, msb);
    memory->Write(registers.sp--, lsb);

    registers.pc = 0x0018;
}

void CPU::RST_20H()
{
    uint8_t msb = (uint8_t) registers.pc >> 8;
    uint8_t lsb = (uint8_t) registers.pc;

    memory->Write(registers.sp--, msb);
    memory->Write(registers.sp--, lsb);

    registers.pc = 0x0020;
}

void CPU::RST_28H()
{
    uint8_t msb = (uint8_t) registers.pc >> 8;
    uint8_t lsb = (uint8_t) registers.pc;

    memory->Write(registers.sp--, msb);
    memory->Write(registers.sp--, lsb);

    registers.pc = 0x0028;
}

void CPU::RST_30H()
{
    uint8_t msb = (uint8_t) registers.pc >> 8;
    uint8_t lsb = (uint8_t) registers.pc;

    memory->Write(registers.sp--, msb);
    memory->Write(registers.sp--, lsb);

    registers.pc = 0x0030;
}

void CPU::RST_38H()
{
    uint8_t msb = (uint8_t) registers.pc >> 8;
    uint8_t lsb = (uint8_t) registers.pc;

    memory->Write(registers.sp--, msb);
    memory->Write(registers.sp--, lsb);

    registers.pc = 0x0038;
}

void CPU::RET()
{
    auto lsb = memory->Read(registers.sp++);
    auto msb = memory->Read(registers.sp++);

    registers.pc = (uint16_t)(msb << 8 | lsb);
}

void CPU::RET_NZ()
{
    if(!IsZeroFlagSet())
    {
        auto lsb = memory->Read(registers.sp++);
        auto msb = memory->Read(registers.sp++);

        registers.pc = (uint16_t)(msb << 8 | lsb);
    }
}

void CPU::RET_Z()
{
    if(IsZeroFlagSet())
    {
        auto lsb = memory->Read(registers.sp++);
        auto msb = memory->Read(registers.sp++);

        registers.pc = (uint16_t)(msb << 8 | lsb);
    }
}

void CPU::RET_NC()
{
    if(!IsCarryFlagSet())
    {
        auto lsb = memory->Read(registers.sp++);
        auto msb = memory->Read(registers.sp++);

        registers.pc = (uint16_t)(msb << 8 | lsb);
    }
}

void CPU::RET_C()
{
    if(IsCarryFlagSet())
    {
        auto lsb = memory->Read(registers.sp++);
        auto msb = memory->Read(registers.sp++);

        registers.pc = (uint16_t)(msb << 8 | lsb);
    }
}

void CPU::RETI()
{
    auto lsb = memory->Read(registers.sp++);
    auto msb = memory->Read(registers.sp++);

    registers.pc = (uint16_t)(msb << 8 | lsb);
    interruptState = InterruptState::Enabled;
}




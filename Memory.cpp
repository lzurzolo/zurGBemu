//
// Created by lorenzo on 8/16/20.
//
#include "Memory.hpp"

// Memory map

// 0xFFFF interrupt enable register
const uint16_t INTERRUPT_ENABLE_REGISTER = 0xFFFF;

// 0xFF80 - 0xFFFE internal ram
const uint16_t INTERNAL_RAM_2_START = 0xFF80;
const uint16_t INTERNAL_RAM_2_END = 0xFFFE;

// 0xFF4C - 0xFF7F empty but unusable for I/O
const uint16_t EMPTY_BLOCK_2_START = 0xFF4C;
const uint16_t EMPTY_BLOCK_2_END = 0xFF7F;

// 0xFF00 - 0xFF4B I/O ports
const uint16_t IO_START = 0xFF00;
const uint16_t IO_END = 0xFF4B;

// 0xFEA0 - 0xFEFF empty but unusable for I/O
const uint16_t EMPTY_BLOCK_1_START = 0xFEA0;
const uint16_t EMPTY_BLOCK_1_END = 0xFEFF;

// 0xFE00 - 0xFE9F sprite attribute memory (OAM)
const uint16_t SPRITE_ATTRIBUTE_MEMORY_START = 0xFE00;
const uint16_t SPRITE_ATTRIBUTE_MEMORY_END = 0xFE9F;

// 0xE000 - 0xFDFF 8kB echo of internal ram
const uint16_t INTERNAL_RAM_ECHO_START = 0xE000;
const uint16_t INTERNAL_RAM_ECHO_END = 0xFDFF;

// 0xC000 - 0xDFFF 8kB internal ram
const uint16_t INTERNAL_RAM_1_START = 0xC000;
const uint16_t INTERNAL_RAM_1_END = 0xDFFF;

// 0xA000 - 0xBFFF 8kB switchable ram
const uint16_t SWITCHABLE_RAM_START = 0xA000;
const uint16_t SWITCHABLE_RAM_END = 0xBFFF;

// 0x8000 - 0x9FFF 8kB vram
const uint16_t VRAM_START = 0x8000;
const uint16_t VRAM_END = 0x9FFF;

// cartridge rom

// 0x4000 - 0x7FFF - 16kB switchable ROM bank
const uint16_t SWITCHABLE_ROM_BANK_START = 0x4000;
const uint16_t SWITCHABLE_ROM_BANK_END = 0x7FFF;

// 0x0000 - 0x3FFF - 16kB ROM bank #0
const uint16_t ROM_BANK_0_START = 0x0000;
const uint16_t ROM_BANK_0_END = 0x3FFF;

Memory::Memory()
{
    Reset();
}

Memory::~Memory()
= default;

uint8_t Memory::Read(uint16_t address)
{
    return bytes[address];
}

void Memory::Write(uint16_t address, uint8_t data)
{
    assert(address >= 0x8000); // anything below 0x8000 is cartridge rom and cannot be written to
    bytes[address] = data;
    if(address >= INTERNAL_RAM_1_START && address <= INTERNAL_RAM_1_END) bytes[address + 0x2000] = data;
    if(address >= INTERNAL_RAM_ECHO_START && address <= INTERNAL_RAM_ECHO_END) bytes[address - 0x2000] = data;
}

void Memory::SetRomIntoMemory(const std::vector<uint8_t> &rom)
{
    std::copy(rom.begin(), rom.end(), bytes.begin());
}

void Memory::Reset()
{
    std::fill(bytes.begin() + 0x8000, bytes.end(), 0);

    Write(0xFF05, 0x00);
    Write(0xFF06, 0x00);
    Write(0xFF07, 0x00);
    Write(0xFF10, 0x80);
    Write(0xFF11, 0xBF);
    Write(0xFF12, 0xF3);
    Write(0xFF14, 0xBF);
    Write(0xFF16, 0x3F);
    Write(0xFF17, 0x00);
    Write(0xFF19, 0xBF);
    Write(0xFF1A, 0x7F);
    Write(0xFF1B, 0xFF);
    Write(0xFF1C, 0x9F);
    Write(0xFF1E, 0xBF);
    Write(0xFF20, 0xFF);
    Write(0xFF21, 0x00);
    Write(0xFF22, 0x00);
    Write(0xFF23, 0xBF);
    Write(0xFF24, 0x77);
    Write(0xFF25, 0xF3);
    Write(0xFF26, 0xF1);
    Write(0xFF40, 0x91);
    Write(0xFF42, 0x00);
    Write(0xFF43, 0x00);
    Write(0xFF45, 0x00);
    Write(0xFF47, 0xFC);
    Write(0xFF48, 0xFF);
    Write(0xFF49, 0xFF);
    Write(0xFF4A, 0x00);
    Write(0xFF4B, 0x00);
    Write(0xFFFF, 0x00);
}



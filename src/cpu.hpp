#pragma once

#include <cstdint>
#include <vector>

#include "addressing_mode.hpp"

constexpr std::uint16_t MEMORY_SIZE = 0xFFFF;
constexpr std::uint16_t ADDRESS_PROGRAM = 0x8000;
constexpr std::uint16_t ADDRESS_PROGRAM_COUNTER = 0xFFFC;

class CPU {
public:
    std::uint8_t _status;
    std::uint16_t _programCounter;

    std::uint8_t _registerA;
    std::uint8_t _registerX;
    std::uint8_t _registerY;

    std::uint8_t _memory[MEMORY_SIZE];

    std::uint8_t _MemRead(std::uint16_t addr);
    std::uint16_t _MemRead16(std::uint16_t addr);

    void _MemWrite(std::uint16_t addr, std::uint8_t data);
    void _MemWrite16(std::uint16_t addr, std::uint16_t data);

    std::uint16_t _GetOperandAddress(AddressingMode mode);

    void _UpdateZeroAndNegativeFlags(std::uint8_t result);

    void _Lda(AddressingMode mode);
    void _Tax();
    void _Inx();
    void _Sta(AddressingMode mode);

    CPU();

    void Load(const std::vector<std::uint8_t>& program);
    void Prepare();

    void Run();
    void Run(const std::vector<std::uint8_t>& program);
};

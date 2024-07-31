#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

#include "addressing_mode.hpp"

class OpCode {
public:
    const std::uint8_t code;
    const std::string mnemonic;
    const std::uint8_t size;
    const std::uint8_t cycles;
    const AddressingMode mode;

    OpCode(std::uint8_t code, const std::string& mnemonic, std::uint8_t size, std::uint8_t cycles, AddressingMode mode)
        : code(code)
        , mnemonic(mnemonic)
        , size(size)
        , cycles(cycles)
        , mode(mode)
    {
    }

    OpCode operator=(const OpCode& other)
    {
        return OpCode(other);
    }
};

const std::unordered_map<std::uint8_t, OpCode> OPCODES {
    { 0x00, OpCode(0x00, "BRK", 1, 7, AddressingMode::None) },
    { 0xAA, OpCode(0xAA, "TAX", 1, 2, AddressingMode::None) },
    { 0xE8, OpCode(0xE8, "INX", 1, 2, AddressingMode::None) },

    { 0xA9, OpCode(0xA9, "LDA", 2, 2, AddressingMode::Immediate) },
    { 0xA5, OpCode(0xA5, "LDA", 2, 3, AddressingMode::ZeroPage) },
    { 0xB5, OpCode(0xB5, "LDA", 2, 4, AddressingMode::ZeroPageX) },
    { 0xAD, OpCode(0xAD, "LDA", 3, 4, AddressingMode::Absolute) },
    { 0xBD, OpCode(0xBD, "LDA", 3, 4, AddressingMode::AbsoluteX) }, // ? NOTE: +1 cycles if page crossed
    { 0xB9, OpCode(0xB9, "LDA", 3, 4, AddressingMode::AbsoluteY) }, // ? NOTE: +1 cycles if page crossed
    { 0xA1, OpCode(0xA1, "LDA", 2, 6, AddressingMode::IndirectX) },
    { 0xB1, OpCode(0xB1, "LDA", 2, 5, AddressingMode::IndirectY) } // ? NOTE: +1 cycles if page crossed
};

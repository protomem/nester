#include "cpu.hpp"

#include <stdexcept>

#include "opcode.hpp"

CPU::CPU()
    : _status(0)
    , _programCounter(0)
    , _registerA(0)
    , _registerX(0)
    , _registerY(0)
    , _memory()
{
}

void CPU::Load(const std::vector<std::uint8_t>& program)
{
    for (std::size_t i = 0; i < program.size(); ++i) {
        const std::size_t addr = ADDRESS_PROGRAM + i;
        _memory[addr] = program[i];
    }
    _MemWrite16(ADDRESS_PROGRAM_COUNTER, ADDRESS_PROGRAM);
}

void CPU::Prepare()
{
    _registerA = 0;
    _registerX = 0;
    _registerY = 0;
    _status = 0;
    _programCounter = _MemRead16(ADDRESS_PROGRAM_COUNTER);
}

void CPU::Run()
{
    while (true) {
        const std::uint8_t code = _MemRead(_programCounter);
        const OpCode opcode = OPCODES.at(code); // ? NOTE: Handle not found key

        _programCounter += 1;
        const std::uint16_t programCounterState = _programCounter;

        switch (code) {
        case 0xA9:
        case 0xA5:
        case 0xB5:
        case 0xAD:
        case 0xBD:
        case 0xB9:
        case 0xA1:
        case 0xB1:
            _Lda(opcode.mode);
            break;

        case 0x85:
        case 0x95:
        case 0x8D:
        case 0x9D:
        case 0x99:
        case 0x81:
        case 0x91:
            _Sta(opcode.mode);
            break;

        case 0xAA:
            _Tax();
            break;

        case 0xE8:
            _Inx();
            break;

        case 0x00:
            return;

        default:
            throw std::runtime_error("Unimplemented opcode");
        }

        if (programCounterState == _programCounter) {
            _programCounter += opcode.size - 1;
        }
    }
}

void CPU::Run(const std::vector<std::uint8_t>& program)
{
    Load(program);
    Prepare();
    Run();
}

std::uint8_t CPU::_MemRead(std::uint16_t addr)
{
    return _memory[addr];
}

std::uint16_t CPU::_MemRead16(std::uint16_t addr)
{
    const std::uint16_t lo = _MemRead(addr);
    const std::uint16_t hi = _MemRead(addr + 1);
    return (hi << 8) | lo;
}

void CPU::_MemWrite(std::uint16_t addr, std::uint8_t data)
{
    _memory[addr] = data;
}

void CPU::_MemWrite16(std::uint16_t addr, std::uint16_t data)
{
    const std::uint8_t lo = data & 0xFF;
    const std::uint8_t hi = data >> 8;
    _MemWrite(addr, lo);
    _MemWrite(addr + 1, hi);
}

std::uint16_t CPU::_GetOperandAddress(AddressingMode mode)
{
    switch (mode) {
    case AddressingMode::Immediate:
        return _programCounter;
    case AddressingMode::ZeroPage:
        return _MemRead(_programCounter);
    case AddressingMode::Absolute:
        return _MemRead16(_programCounter);

    case AddressingMode::ZeroPageX: {
        const std::uint8_t base = _MemRead(_programCounter);
        return (base + _registerX);
    }

    case AddressingMode::ZeroPageY: {
        const std::uint8_t base = _MemRead(_programCounter);
        return (base + _registerY);
    }

    case AddressingMode::AbsoluteX: {
        const std::uint16_t base = _MemRead16(_programCounter);
        return (base + _registerX);
    }

    case AddressingMode::AbsoluteY: {
        const std::uint16_t base = _MemRead16(_programCounter);
        return (base + _registerY);
    }

    case AddressingMode::IndirectX: {
        std::uint8_t base = _MemRead(_programCounter);
        base += _registerX;

        const std::uint16_t lo = _MemRead(base);
        const std::uint16_t hi = _MemRead(base + 1);

        return hi << 8 | lo;
    }

    case AddressingMode::IndirectY: {
        const std::uint8_t base = _MemRead(_programCounter);

        const std::uint16_t lo = _MemRead(base);
        const std::uint16_t hi = _MemRead(base + 1);

        const std::uint16_t deref = hi << 8 | lo;
        return deref + _registerY;
    }

    case AddressingMode::None:
    default:
        throw std::runtime_error("Unsupported addressing mode");
    }
}

void CPU::_UpdateZeroAndNegativeFlags(std::uint8_t result)
{
    if (result == 0) {
        _status |= 0b00000010;
    } else {
        _status &= 0b11111101;
    }

    if ((result & 0b10000000) != 0) {
        _status |= 0b10000000;
    } else {
        _status &= 0b01111111;
    }
}

void CPU::_Lda(AddressingMode mode)
{
    const std::uint16_t addr = _GetOperandAddress(mode);
    const std::uint8_t data = _MemRead(addr);

    _registerA = data;
    _UpdateZeroAndNegativeFlags(data);
}

void CPU::_Sta(AddressingMode mode)
{
    const std::uint16_t addr = _GetOperandAddress(mode);
    _MemWrite(addr, _registerA);
}

void CPU::_Tax()
{
    _registerX = _registerA;
    _UpdateZeroAndNegativeFlags(_registerX);
}

void CPU::_Inx()
{
    _registerX += 1;
    _UpdateZeroAndNegativeFlags(_registerX);
}

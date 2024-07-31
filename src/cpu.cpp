#include "cpu.hpp"

#include "opcode.hpp"

CPU::CPU()
    : _status(0)
    , _program_counter(0)
    , _register_a(0)
    , _register_x(0)
{
}

std::uint8_t CPU::Run(const std::vector<std::uint8_t>& program)
{
    while (true) {
        auto opcode = static_cast<OpCodes>(program[this->_program_counter]);
        this->_program_counter++;

        switch (opcode) {
        case OpCodes::BRK:
            goto EXIT_LOOP;

        case OpCodes::TAX:
            _Tax();
            break;

        case OpCodes::INX:
            _Inx();
            break;

        case OpCodes::LDA:
            const auto param = program[_program_counter];
            _program_counter++;

            _Lda(param);
            break;
        }
    }
EXIT_LOOP:

    return _status;
}

void CPU::_Lda(std::uint8_t param)
{
    _register_a = param;
    _UpdateZeroAndNegativeFlags(param);
}

void CPU::_Tax()
{
    _register_x = _register_a;
    _UpdateZeroAndNegativeFlags(_register_x);
}

void CPU::_Inx()
{
    _register_x++;
    _UpdateZeroAndNegativeFlags(_register_x);
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

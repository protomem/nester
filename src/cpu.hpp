#pragma once

#include <cstdint>
#include <vector>

class CPU {
private:
    std::uint8_t _status;
    std::uint16_t _program_counter;

    std::uint8_t _register_a;
    std::uint8_t _register_x;

    void _Lda(std::uint8_t param);
    void _Tax();
    void _Inx();

    void _UpdateZeroAndNegativeFlags(std::uint8_t result);

public:
    CPU();

    std::uint8_t Run(const std::vector<std::uint8_t>& program);
};

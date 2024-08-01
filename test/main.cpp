#include <gtest/gtest.h>

#include "../src/cpu.hpp"

TEST(CPU, Test_0xA9_LDA_Immediate)
{
    CPU cpu;
    cpu.Run({ 0xA9, 0x05, 0x00 });

    ASSERT_EQ(cpu._registerA, 5);
    ASSERT_EQ(cpu._status & 0b00000010, 0);
    ASSERT_EQ(cpu._status & 0b10000000, 0);
}

TEST(CPU, Test_LDA_From_Memory)
{
    CPU cpu;
    cpu.Load({ 0xa5, 0x10, 0x00 });
    cpu.Prepare();
    cpu._MemWrite(0x10, 0x55);
    cpu.Run();

    ASSERT_EQ(cpu._registerA, 0x55);
}

TEST(CPU, Test_0xAA_TAX)
{
    CPU cpu;
    cpu.Run({ 0xA9, 0x0A, 0xAA, 0x00 });

    ASSERT_EQ(cpu._registerA, 10);
    ASSERT_EQ(cpu._registerX, 10);
}

TEST(CPU, Test_0xE8_Inx_Overflow)
{
    CPU cpu;
    cpu.Run({ 0xA9, 0xFF, 0xAA, 0xE8, 0xE8, 0x00 });

    ASSERT_EQ(cpu._registerX, 1);
}

TEST(CPU, Test_Working_Together)
{
    CPU cpu;
    cpu.Run({ 0xA9, 0xC0, 0xAA, 0xE8, 0x00 });

    ASSERT_EQ(cpu._registerX, 0xC1);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

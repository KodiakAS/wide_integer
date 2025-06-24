#include <gtest/gtest.h>
#include <wide_integer/wide_integer.h>

TEST(WideIntegerBasic, Addition)
{
    wide::integer<128, unsigned> a = 1;
    wide::integer<128, unsigned> b = 2;
    auto c = a + b;
    EXPECT_EQ(wide::to_string(c), "3");
}

TEST(WideIntegerBasic, Multiplication)
{
    wide::integer<128, unsigned> a = 10;
    wide::integer<128, unsigned> b = 20;
    auto c = a * b;
    EXPECT_EQ(wide::to_string(c), "200");
}

TEST(WideIntegerBasic, Division)
{
    wide::integer<128, unsigned> a = 200;
    wide::integer<128, unsigned> b = 10;
    auto c = a / b;
    EXPECT_EQ(wide::to_string(c), "20");
}

TEST(WideIntegerBasic, Negative)
{
    wide::integer<128, signed> a = -5;
    wide::integer<128, signed> b = 2;
    auto c = a + b;
    EXPECT_EQ(wide::to_string(c), "-3");
}

TEST(WideIntegerBasic, Subtraction)
{
    wide::integer<128, unsigned> a = 100;
    wide::integer<128, unsigned> b = 40;
    auto c = a - b;
    EXPECT_EQ(wide::to_string(c), "60");
}

TEST(WideIntegerBasic, ShiftLeft)
{
    wide::integer<128, unsigned> a = 1;
    auto c = a << 100;
    EXPECT_EQ(wide::to_string(c), "1267650600228229401496703205376");
}

TEST(WideIntegerBasic, Bitwise)
{
    wide::integer<128, unsigned> a = 10; // 1010
    wide::integer<128, unsigned> b = 12; // 1100
    auto c = a & b;
    EXPECT_EQ(wide::to_string(c), "8");
    auto d = a | b;
    EXPECT_EQ(wide::to_string(d), "14");
}


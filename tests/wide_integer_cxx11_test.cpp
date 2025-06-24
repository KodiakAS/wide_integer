#include <gtest/gtest.h>
#include <wide_integer/wide_integer_cxx11.h>

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
TEST(WideIntegerAdditional, ShiftRight)
{
    wide::integer<128, unsigned> a = wide::integer<128, unsigned>(1) << 127;
    auto b = a >> 64;
    EXPECT_EQ(wide::to_string(b), "9223372036854775808");
}

TEST(WideIntegerAdditional, BitwiseNot)
{
    wide::integer<128, unsigned> a = 0;
    auto b = ~a;
    EXPECT_EQ(wide::to_string(b), "340282366920938463463374607431768211455");
}

TEST(WideIntegerAdditional, Comparison)
{
    wide::integer<128, unsigned> a = 5;
    wide::integer<128, unsigned> b = 10;
    EXPECT_TRUE(a < b);
    EXPECT_TRUE(b > a);
    EXPECT_TRUE(a <= a);
    EXPECT_TRUE(b >= a);
}

TEST(WideIntegerBoundary, Unsigned256)
{
    wide::integer<256, unsigned> a = wide::integer<256, unsigned>(1) << 255;
    EXPECT_EQ(wide::to_string(a), "57896044618658097711785492504343953926634992332820282019728792003956564819968");
    wide::integer<256, unsigned> max = ~wide::integer<256, unsigned>(0);
    EXPECT_EQ(wide::to_string(max), "115792089237316195423570985008687907853269984665640564039457584007913129639935");
    auto zero = max + wide::integer<256, unsigned>(1);
    EXPECT_EQ(wide::to_string(zero), "0");
}

TEST(WideIntegerBoundary, Signed256)
{
    wide::integer<256, signed> a = -(wide::integer<256, signed>(1) << 255);
    EXPECT_EQ(wide::to_string(a), "-57896044618658097711785492504343953926634992332820282019728792003956564819968");
}

TEST(WideIntegerBoundary, Unsigned512)
{
    wide::integer<512, unsigned> a = wide::integer<512, unsigned>(1) << 511;
    EXPECT_EQ(
        wide::to_string(a),
        "6703903964971298549787012499102923063739682910296196688861780721860882015036773488400937149083451713845015929093243025426876941405"
        "973284973216824503042048");
}

TEST(WideIntegerBoundary, Signed512)
{
    wide::integer<512, signed> a = -(wide::integer<512, signed>(1) << 511);
    EXPECT_EQ(
        wide::to_string(a),
        "-670390396497129854978701249910292306373968291029619668886178072186088201503677348840093714908345171384501592909324302542687694140"
        "5973284973216824503042048");
}

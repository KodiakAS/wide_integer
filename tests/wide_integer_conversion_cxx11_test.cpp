#include <gtest/gtest.h>
#include <wide_integer/wide_integer_cxx11.h>

TEST(WideIntegerConversion, UnsignedRoundtrip)
{
    wide::integer<128, unsigned> w = 42;
    uint64_t u = w;
    EXPECT_EQ(u, 42u);
    wide::integer<128, unsigned> w2 = u;
    EXPECT_EQ(w2, w);
}

TEST(WideIntegerConversion, SignedRoundtrip)
{
    wide::integer<128, signed> w = -123;
    int64_t i = w;
    EXPECT_EQ(i, -123);
    wide::integer<128, signed> w2 = i;
    EXPECT_EQ(w2, w);
}

TEST(WideIntegerConversion, ArithmeticWithBuiltin)
{
    wide::integer<128, unsigned> a = 100;
    uint64_t b = 20;
    auto c = a + b;
    auto d = b + a;
    auto e = a * b;
    EXPECT_EQ(wide::to_string(c), "120");
    EXPECT_EQ(wide::to_string(d), "120");
    EXPECT_EQ(wide::to_string(e), "2000");
}

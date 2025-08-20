#include <cstdint>
#include <fmt/format.h>
#include <gtest/gtest.h>

#ifdef USE_CXX11_HEADER
#    include <wide_integer/wide_integer_cxx11.h>
#else
#    include <wide_integer/wide_integer.h>
#endif

struct A
{
};
struct B
{
};

template <typename, typename>
struct Base;

template <>
struct Base<A, B>
{
    static constexpr wide::integer<256, signed> val = 1;
};

static_assert(Base<A, B>::val == 1, "template static constexpr initialization failed");

TEST(WideIntegerConstexpr, Construction)
{
#if __cplusplus >= 201402L
    constexpr wide::integer<128, unsigned> a = 42;
    static_assert(a == 42, "constexpr constructor failed");
    constexpr wide::integer<128, signed> b = -42;
    static_assert(b == -42, "constexpr constructor failed");
    constexpr wide::integer<256, unsigned> c = 1;
    static_assert(c == 1, "constexpr construction failed");
    constexpr wide::integer<256, signed> d = -1;
    static_assert(d == -1, "constexpr construction failed");
    (void)a;
    (void)b;
    (void)c;
    (void)d;
#else
    SUCCEED();
#endif
}

enum class ArithOp
{
    Add,
    Sub,
    Mul,
    Div,
    Mod
};

struct ArithCase
{
    ArithOp op;
    unsigned a;
    unsigned b;
    const char * expected;
};

static std::string arith_case_name(const testing::TestParamInfo<ArithCase> & info)
{
    std::string op;
    switch (info.param.op)
    {
        case ArithOp::Add:
            op = "Add";
            break;
        case ArithOp::Sub:
            op = "Sub";
            break;
        case ArithOp::Mul:
            op = "Mul";
            break;
        case ArithOp::Div:
            op = "Div";
            break;
        case ArithOp::Mod:
            op = "Mod";
            break;
    }
    return fmt::format("{}_{}_{}", op, info.param.a, info.param.b);
}

class WideIntegerArithmeticTest : public ::testing::TestWithParam<ArithCase>
{
};

TEST_P(WideIntegerArithmeticTest, BasicOps)
{
    const auto param = GetParam();
    wide::integer<128, unsigned> a = param.a;
    wide::integer<128, unsigned> b = param.b;
    wide::integer<128, unsigned> c = 0;
    switch (param.op)
    {
        case ArithOp::Add:
            c = a + b;
            break;
        case ArithOp::Sub:
            c = a - b;
            break;
        case ArithOp::Mul:
            c = a * b;
            break;
        case ArithOp::Div:
            c = a / b;
            break;
        case ArithOp::Mod:
            c = a % b;
            break;
    }
    EXPECT_EQ(wide::to_string(c), param.expected);
}

INSTANTIATE_TEST_SUITE_P(
    WideIntegerBasic,
    WideIntegerArithmeticTest,
    ::testing::Values(
        ArithCase{ArithOp::Add, 1, 2, "3"},
        ArithCase{ArithOp::Sub, 100, 40, "60"},
        ArithCase{ArithOp::Mul, 10, 20, "200"},
        ArithCase{ArithOp::Div, 200, 10, "20"},
        ArithCase{ArithOp::Mod, 200, 30, "20"}),
    arith_case_name);

TEST(WideIntegerBasic, SignedArithmetic)
{
    wide::integer<128, signed> a = -5;
    wide::integer<128, signed> b = 2;
    auto c = a + b;
    EXPECT_EQ(wide::to_string(c), "-3");
}

TEST(WideIntegerBasic, Shift)
{
    wide::integer<128, unsigned> a = 1;
    auto left = a << 100;
    EXPECT_EQ(wide::to_string(left), "1267650600228229401496703205376");
    auto b = wide::integer<128, unsigned>(1) << 127;
    auto right = b >> 64;
    EXPECT_EQ(wide::to_string(right), "9223372036854775808");
}

enum class BitOp
{
    And,
    Or,
    Xor
};

struct BitCase
{
    BitOp op;
    unsigned a;
    unsigned b;
    const char * expected;
};

static std::string bit_case_name(const testing::TestParamInfo<BitCase> & info)
{
    std::string op;
    switch (info.param.op)
    {
        case BitOp::And:
            op = "And";
            break;
        case BitOp::Or:
            op = "Or";
            break;
        case BitOp::Xor:
            op = "Xor";
            break;
    }
    return fmt::format("{}_{}_{}", op, info.param.a, info.param.b);
}

class WideIntegerBitwiseTest : public ::testing::TestWithParam<BitCase>
{
};

TEST_P(WideIntegerBitwiseTest, BasicOps)
{
    const auto param = GetParam();
    wide::integer<128, unsigned> a = param.a;
    wide::integer<128, unsigned> b = param.b;
    wide::integer<128, unsigned> c = 0;
    switch (param.op)
    {
        case BitOp::And:
            c = a & b;
            break;
        case BitOp::Or:
            c = a | b;
            break;
        case BitOp::Xor:
            c = a ^ b;
            break;
    }
    EXPECT_EQ(wide::to_string(c), param.expected);
}

INSTANTIATE_TEST_SUITE_P(
    WideIntegerBasic,
    WideIntegerBitwiseTest,
    ::testing::Values(BitCase{BitOp::And, 10, 12, "8"}, BitCase{BitOp::Or, 10, 12, "14"}, BitCase{BitOp::Xor, 10, 12, "6"}),
    bit_case_name);

TEST(WideIntegerConversion, BuiltinToWide)
{
    int a = -42;
    wide::integer<128, signed> b = a; // implicit from int
    EXPECT_EQ(wide::to_string(b), "-42");

    unsigned long long c = 42;
    wide::integer<128, unsigned> d = c; // implicit from unsigned long long
    EXPECT_EQ(wide::to_string(d), "42");
}

TEST(WideIntegerConversion, WideToBuiltin)
{
    wide::integer<128, unsigned> a = 100;
    unsigned int b = a; // implicit conversion to builtin
    EXPECT_EQ(b, 100u);

    wide::integer<128, signed> c = -100;
    int d = c;
    EXPECT_EQ(d, -100);
}

TEST(WideIntegerConversion, MixedArithmetic)
{
    wide::integer<128, unsigned> a = 100;
    unsigned int b = 50;
    auto c = a + b; // builtin implicitly converted to wide integer
    EXPECT_EQ(wide::to_string(c), "150");
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

TEST(WideIntegerAdditional, BuiltinComparison)
{
    wide::integer<128, unsigned> a = 5;
    int32_t iv = 10;
    int64_t lv = 3;
    EXPECT_TRUE(a < iv);
    EXPECT_TRUE(lv < a);
    EXPECT_TRUE(iv > a);
    EXPECT_TRUE(a >= lv);
    wide::integer<128, signed> b = -5;
    int32_t neg = -10;
    EXPECT_TRUE(b < 0);
    EXPECT_TRUE(0 > b);
    EXPECT_TRUE(neg < b);
    EXPECT_TRUE(b >= neg);
}

TEST(WideIntegerComparison, Floating)
{
    wide::integer<128, signed> a = -5;
    wide::integer<128, unsigned> b = 5;
    float f = -5.0f;
    double d = 10.0;
    EXPECT_TRUE(a == f);
    EXPECT_TRUE(f == a);
    EXPECT_TRUE(b < d);
    EXPECT_TRUE(d > b);
    EXPECT_TRUE(a <= 0.0);
    EXPECT_TRUE(0.0 >= a);
}

TEST(WideIntegerConversion, SmallIntegral)
{
    int8_t i8 = -7;
    wide::integer<128, signed> a = i8;
    EXPECT_EQ(static_cast<int8_t>(a), i8);
    uint16_t u16 = 60000;
    wide::integer<128, unsigned> b = u16;
    EXPECT_EQ(static_cast<uint16_t>(b), u16);
}

TEST(WideIntegerConversion, FloatingPoint)
{
    float f = 42.5f;
    wide::integer<128, unsigned> a = f;
    EXPECT_EQ(wide::to_string(a), "42");
    wide::integer<128, unsigned> b = 100;
    EXPECT_FLOAT_EQ(static_cast<float>(b), 100.0f);
    double d = -100.25;
    wide::integer<128, signed> c = d;
    EXPECT_EQ(wide::to_string(c), "-100");
    wide::integer<128, signed> e = -50;
    EXPECT_DOUBLE_EQ(static_cast<double>(e), -50.0);
}

#if defined(__SIZEOF_INT128__)
TEST(WideIntegerConversion, Int128)
{
    __int128 v = (static_cast<__int128>(1) << 100) + 123;
    wide::integer<256, unsigned> a = v;
    EXPECT_EQ(static_cast<__int128>(a), v);
    EXPECT_TRUE(a == v);
    EXPECT_TRUE(v == a);
}
#endif

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

TEST(WideInteger256, Arithmetic)
{
    wide::integer<256, unsigned> base = wide::integer<256, unsigned>(1) << 200;
    wide::integer<256, unsigned> small = 123456789U;

    auto add = base + small;
    EXPECT_EQ(wide::to_string(add), "1606938044258990275541962092341162602522202993782792958758165");

    auto sub = base - small;
    EXPECT_EQ(wide::to_string(sub), "1606938044258990275541962092341162602522202993782792711844587");

    auto mul = small * 20U;
    EXPECT_EQ(wide::to_string(mul), "2469135780");

    auto div = mul / 10U;
    EXPECT_EQ(wide::to_string(div), "246913578");
}

TEST(WideInteger256, BitwiseAndShift)
{
    wide::integer<256, unsigned> a = wide::integer<256, unsigned>(1) << 200;
    auto left = a << 50;
    EXPECT_EQ(wide::to_string(left), "1809251394333065553493296640760748560207343510400633813116524750123642650624");

    auto right = a >> 100;
    EXPECT_EQ(wide::to_string(right), "1267650600228229401496703205376");

    auto b = a | (wide::integer<256, unsigned>(1) << 199);
    EXPECT_EQ(wide::to_string(b), "2410407066388485413312943138511743903783304490674189252952064");
}

TEST(WideInteger256, Comparison)
{
    wide::integer<256, unsigned> a = wide::integer<256, unsigned>(1) << 200;
    wide::integer<256, unsigned> b = wide::integer<256, unsigned>(1) << 199;

    EXPECT_TRUE(a > b);
    EXPECT_TRUE(b < a);
    EXPECT_TRUE(a >= a);
    EXPECT_TRUE(a != b);
}

TEST(WideInteger512, Arithmetic)
{
    wide::integer<512, unsigned> base = wide::integer<512, unsigned>(1) << 400;
    wide::integer<512, unsigned> small = 123456789U;

    auto add = base + small;
    EXPECT_EQ(
        wide::to_string(add),
        "258224987808690858965591917200301187432970579282922351283065935654064762"
        "2016841194629645353280137831435903171972870950165");

    auto sub = base - small;
    EXPECT_EQ(
        wide::to_string(sub),
        "258224987808690858965591917200301187432970579282922351283065935654064762"
        "2016841194629645353280137831435903171972624036587");

    auto mul = base * small;
    EXPECT_EQ(
        wide::to_string(mul),
        "318796278344251197415438395819030544333617004497395160257373504911314503"
        "394684917811900059524736434146499864926670201039999729664");

    auto div = base / small;
    EXPECT_EQ(
        wide::to_string(div),
        "209162242028415998220715036740750796161538414289166674570700146473163790"
        "28915462839630839202209753174739569583109");

    auto lshift = base << 10;
    EXPECT_EQ(
        wide::to_string(lshift),
        "264422387516099439580766123213108415931361873185712487713859518109762316"
        "4945245383300756841758861139390364848100093433217024");

    auto rshift = base >> 10;
    EXPECT_EQ(
        wide::to_string(rshift),
        "252172839656924666958585856640919128352510331330978858674869077787172619"
        "3375821479130513040312634601011624191379636224");
}

TEST(WideInteger512, BitwiseOps)
{
    wide::integer<512, unsigned> a = wide::integer<512, unsigned>(1) << 500;
    wide::integer<512, unsigned> b = wide::integer<512, unsigned>(1) << 400;

    auto orv = a | b;
    EXPECT_EQ(
        wide::to_string(orv),
        "327339060789614187001318969683018140209472895463272070865529437997046350"
        "2197503778396100751682444804772903525322189716362497394377321296225301275082752");

    auto andv = a & b;
    EXPECT_EQ(wide::to_string(andv), "0");

    auto xorv = a ^ b;
    EXPECT_EQ(
        wide::to_string(xorv),
        "327339060789614187001318969683018140209472895463272070865529437997046350"
        "2197503778396100751682444804772903525322189716362497394377321296225301275082752");
}

TEST(WideInteger512, Comparison)
{
    wide::integer<512, unsigned> a = wide::integer<512, unsigned>(1) << 500;
    wide::integer<512, unsigned> b = wide::integer<512, unsigned>(1) << 400;

    EXPECT_TRUE(a > b);
    EXPECT_TRUE(b < a);
    EXPECT_TRUE(a != b);
    EXPECT_TRUE(a >= a);
}

TEST(WideIntegerExtra, CompoundOperators)
{
    wide::integer<128, unsigned> v = 1;
    v += 2;
    EXPECT_EQ(v, 3U);
    v *= 5;
    EXPECT_EQ(v, 15U);
    v -= 5;
    EXPECT_EQ(v, 10U);
    v /= 2;
    EXPECT_EQ(v, 5U);
    v %= 2U;
    EXPECT_EQ(v, 1U);
    v |= wide::integer<128, unsigned>(2U);
    EXPECT_EQ(v, 3U);
    v &= wide::integer<128, unsigned>(1U);
    EXPECT_EQ(v, 1U);
    v ^= wide::integer<128, unsigned>(3U);
    EXPECT_EQ(v, 2U);
    v <<= 4;
    EXPECT_EQ(v, 32U);
    v >>= 1;
    EXPECT_EQ(v, 16U);
}

TEST(WideIntegerExtra, IncDecAndBool)
{
    wide::integer<128, unsigned> v = 0;
    EXPECT_FALSE(static_cast<bool>(v));
    ++v;
    EXPECT_EQ(v, 1U);
    v++;
    EXPECT_EQ(v, 2U);
    --v;
    EXPECT_EQ(v, 1U);
    v--;
    EXPECT_EQ(v, 0U);
    EXPECT_FALSE(static_cast<bool>(v));
}

TEST(WideIntegerExtra, UnaryAndToString)
{
    wide::integer<128, signed> a = -1;
    auto b = -a;
    EXPECT_EQ(b, 1);

    auto c = +b;
    EXPECT_EQ(c, 1);
    auto d = ~wide::integer<128, unsigned>(0);
    EXPECT_EQ(d, (wide::integer<128, unsigned>(-1)));

    std::ostringstream oss;
    oss << wide::to_string(b);
    EXPECT_EQ(oss.str(), "1");

    EXPECT_EQ(fmt::format("{}", b), "1");
}

TEST(WideIntegerExtra, LargeToString)
{
    auto v = (wide::integer<128, unsigned>(1) << 127) - 1;
    EXPECT_EQ(wide::to_string(v), "170141183460469231731687303715884105727");
}

TEST(WideIntegerExtra, FloatConversion)
{
    wide::integer<128, unsigned> v = 123;
    double d = static_cast<double>(v);
    EXPECT_NEAR(d, 123.0, 1e-9);
    float f = static_cast<float>(v);
    EXPECT_NEAR(f, 123.0f, 1e-6f);

    float f2 = 456.0f;
    wide::integer<128, unsigned> w = f2;
    EXPECT_EQ(w, 456U);
    double d2 = -789.0;
    wide::integer<128, signed> s = d2;
    EXPECT_EQ(s, -789);
}

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

TEST(WideIntegerInt128, UnsignedRoundtrip)
{
    unsigned __int128 value = (static_cast<unsigned __int128>(1) << 80) + 42;
    wide::integer<256, unsigned> w = value;
    unsigned __int128 back = w;
    EXPECT_EQ(back, value);
}
TEST(WideIntegerInt128, SignedRoundtrip)
{
    __int128 value = -((static_cast<__int128>(1) << 90) + 77);
    wide::integer<256, signed> w = value;
    __int128 back = w;
    EXPECT_EQ(back, value);
}
TEST(WideIntegerInt128, Arithmetic)
{
    wide::integer<256, unsigned> w = 100;
    unsigned __int128 b = 20;
    auto c = w + b;
    auto d = b + w;
    auto e = w * b;
    EXPECT_EQ(wide::to_string(c), "120");
    EXPECT_EQ(wide::to_string(d), "120");
    EXPECT_EQ(wide::to_string(e), "2000");
}

TEST(WideIntegerInt128, SignedToUnsignedConversion)
{
    wide::integer<256, signed> w = 123;
    unsigned __int128 via_static = static_cast<unsigned __int128>(w);
    unsigned __int128 via_implicit = w;
    EXPECT_TRUE(via_static == static_cast<unsigned __int128>(123));
    EXPECT_TRUE(via_implicit == static_cast<unsigned __int128>(123));

    wide::integer<256, signed> negative = -1;
    unsigned __int128 static_neg = static_cast<unsigned __int128>(negative);
    unsigned __int128 implicit_neg = negative;
    EXPECT_TRUE(static_neg == static_cast<unsigned __int128>(-1));
    EXPECT_TRUE(implicit_neg == static_cast<unsigned __int128>(-1));
}

TEST(WideIntegerInt128, SignedConversion)
{
    wide::integer<256, signed> w = 123;
    __int128 via_static = static_cast<__int128>(w);
    __int128 via_implicit = w;
    EXPECT_TRUE(via_static == static_cast<__int128>(123));
    EXPECT_TRUE(via_implicit == static_cast<__int128>(123));

    wide::integer<256, signed> negative = -1;
    __int128 static_neg = static_cast<__int128>(negative);
    __int128 implicit_neg = negative;
    EXPECT_TRUE(static_neg == static_cast<__int128>(-1));
    EXPECT_TRUE(implicit_neg == static_cast<__int128>(-1));
}

template <typename T>
void test_integral_ops()
{
    using W = wide::integer<256, signed>;
    const __int128 ai = 1000;
    W a = ai;
    T b = static_cast<T>(123);
    __int128 bi = static_cast<__int128>(b);
    EXPECT_EQ(a + b, W(ai + bi));
    EXPECT_EQ(b + a, W(bi + ai));
    EXPECT_EQ(a - b, W(ai - bi));
    EXPECT_EQ(b - a, W(bi - ai));
    EXPECT_EQ(a * b, W(ai * bi));
    EXPECT_EQ(b * a, W(bi * ai));
    EXPECT_EQ(a / b, W(ai / bi));
    EXPECT_EQ(b / a, W(bi / ai));
    EXPECT_EQ(a % b, W(ai % bi));
    EXPECT_EQ(b % a, W(bi % ai));
    EXPECT_EQ(a & b, W(ai & bi));
    EXPECT_EQ(b & a, W(ai & bi));
    EXPECT_EQ(a | b, W(ai | bi));
    EXPECT_EQ(b | a, W(ai | bi));
    EXPECT_EQ(a ^ b, W(ai ^ bi));
    EXPECT_EQ(b ^ a, W(ai ^ bi));
    EXPECT_TRUE(a > b);
    EXPECT_TRUE(b < a);
    EXPECT_TRUE(a >= b);
    EXPECT_TRUE(b <= a);
    EXPECT_TRUE(a != b);
}

template <typename T>
void test_float_ops()
{
    using W = wide::integer<256, signed>;
    const __int128 ai = 1000;
    W a = ai;
    T b = static_cast<T>(123.5);
    __int128 bi = static_cast<__int128>(b);
    EXPECT_EQ(a + b, W(ai + bi));
    EXPECT_EQ(b + a, W(ai + bi));
    EXPECT_EQ(a - b, W(ai - bi));
    EXPECT_EQ(b - a, W(bi - ai));
    EXPECT_EQ(a * b, W(ai * bi));
    EXPECT_EQ(b * a, W(ai * bi));
    EXPECT_EQ(a / b, W(ai / bi));
    EXPECT_EQ(b / a, W(bi / ai));
    EXPECT_EQ(a % b, W(ai % bi));
    EXPECT_EQ(b % a, W(bi % ai));
    EXPECT_TRUE(a > b);
    EXPECT_TRUE(b < a);
    EXPECT_TRUE(a >= b);
    EXPECT_TRUE(b <= a);
    EXPECT_TRUE(a != b);
}
TEST(WideIntegerBuiltin, IntegralTypes)
{
    test_integral_ops<int8_t>();
    test_integral_ops<uint8_t>();
    test_integral_ops<int16_t>();
    test_integral_ops<uint16_t>();
    test_integral_ops<int32_t>();
    test_integral_ops<uint32_t>();
    test_integral_ops<int64_t>();
    test_integral_ops<uint64_t>();
    test_integral_ops<__int128>();
    test_integral_ops<unsigned __int128>();
}

#ifdef USE_CXX11_HEADER
TEST(WideIntegerBuiltin, FloatingTypes)
{
    test_float_ops<float>();
    test_float_ops<double>();
}
#endif

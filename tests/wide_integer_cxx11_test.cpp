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

TEST(WideIntegerExtraCxx11, CompoundOperators)
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


TEST(WideIntegerExtraCxx11, UnaryAndToString)
{
    wide::integer<128, signed> a = -1;
    auto b = -a;
    EXPECT_EQ(b, 1);
    auto d = ~wide::integer<128, unsigned>(0);
    EXPECT_EQ(d, (wide::integer<128, unsigned>(-1)));

    std::ostringstream oss;
    oss << wide::to_string(b);
    EXPECT_EQ(oss.str(), "1");
}

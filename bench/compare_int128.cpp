#include <wide_integer/std_compat.h>

#include <benchmark/benchmark.h>
#include <boost/multiprecision/cpp_int.hpp>

#ifdef USE_CXX11_HEADER
#    include <wide_integer/wide_integer_cxx11.h>
#else
#    include <wide_integer/wide_integer.h>
#endif

using WInt128 = wide::integer<128, unsigned>;
using WInt256 = wide::integer<256, unsigned>;
using BInt128 = boost::multiprecision::uint128_t;
using BInt256 = boost::multiprecision::uint256_t;

static void BM_Wide128Addition(benchmark::State & state)
{
    WInt128 a = 123456789;
    WInt128 b = 987654321;
    for (auto _ : state)
    {
        WInt128 c = a;
        for (int i = 0; i < 1000; ++i)
        {
            c += b;
            benchmark::DoNotOptimize(c);
        }
    }
}

static void BM_Wide128Subtraction(benchmark::State & state)
{
    WInt128 a = 1234567890;
    WInt128 b = 987654321;
    for (auto _ : state)
    {
        WInt128 c = a;
        for (int i = 0; i < 1000; ++i)
        {
            c -= b;
            benchmark::DoNotOptimize(c);
        }
    }
}

static void BM_Builtin128Addition(benchmark::State & state)
{
    unsigned __int128 a = 123456789;
    unsigned __int128 b = 987654321;
    for (auto _ : state)
    {
        unsigned __int128 c = a;
        for (int i = 0; i < 1000; ++i)
        {
            c += b;
            benchmark::DoNotOptimize(c);
        }
    }
}

static void BM_Builtin128Subtraction(benchmark::State & state)
{
    unsigned __int128 a = 1234567890;
    unsigned __int128 b = 987654321;
    for (auto _ : state)
    {
        unsigned __int128 c = a;
        for (int i = 0; i < 1000; ++i)
        {
            c -= b;
            benchmark::DoNotOptimize(c);
        }
    }
}

static void BM_Wide128Multiplication(benchmark::State & state)
{
    WInt128 a = 123456789;
    WInt128 b = 987654321;
    for (auto _ : state)
    {
        WInt128 c = a;
        for (int i = 0; i < 100; ++i)
        {
            c *= b;
            benchmark::DoNotOptimize(c);
        }
    }
}

static void BM_Wide128Division(benchmark::State & state)
{
    WInt128 a = 1234567890123456789ULL;
    WInt128 b = 987654321;
    for (auto _ : state)
    {
        WInt128 c = a;
        for (int i = 0; i < 100; ++i)
        {
            c /= b;
            benchmark::DoNotOptimize(c);
        }
    }
}

static void BM_Builtin128Multiplication(benchmark::State & state)
{
    unsigned __int128 a = 123456789;
    unsigned __int128 b = 987654321;
    for (auto _ : state)
    {
        unsigned __int128 c = a;
        for (int i = 0; i < 100; ++i)
        {
            c *= b;
            benchmark::DoNotOptimize(c);
        }
    }
}

static void BM_Builtin128Division(benchmark::State & state)
{
    unsigned __int128 a = static_cast<unsigned __int128>(1234567890123456789ULL) << 64;
    unsigned __int128 b = 987654321;
    for (auto _ : state)
    {
        unsigned __int128 c = a;
        for (int i = 0; i < 100; ++i)
        {
            c /= b;
            benchmark::DoNotOptimize(c);
        }
    }
}

static void BM_Boost128Addition(benchmark::State & state)
{
    BInt128 a = 123456789;
    BInt128 b = 987654321;
    for (auto _ : state)
    {
        BInt128 c = a;
        for (int i = 0; i < 1000; ++i)
        {
            c += b;
            benchmark::DoNotOptimize(c);
        }
    }
}

static void BM_Boost128Subtraction(benchmark::State & state)
{
    BInt128 a = 1234567890;
    BInt128 b = 987654321;
    for (auto _ : state)
    {
        BInt128 c = a;
        for (int i = 0; i < 1000; ++i)
        {
            c -= b;
            benchmark::DoNotOptimize(c);
        }
    }
}

static void BM_Boost128Multiplication(benchmark::State & state)
{
    BInt128 a = 123456789;
    BInt128 b = 987654321;
    for (auto _ : state)
    {
        BInt128 c = a;
        for (int i = 0; i < 100; ++i)
        {
            c *= b;
            benchmark::DoNotOptimize(c);
        }
    }
}

static void BM_Boost128Division(benchmark::State & state)
{
    BInt128 a = 1234567890123456789ULL;
    BInt128 b = 987654321;
    for (auto _ : state)
    {
        BInt128 c = a;
        for (int i = 0; i < 100; ++i)
        {
            c /= b;
            benchmark::DoNotOptimize(c);
        }
    }
}

static void BM_Wide256Addition(benchmark::State & state)
{
    WInt256 a = 123456789;
    WInt256 b = 987654321;
    for (auto _ : state)
    {
        WInt256 c = a;
        for (int i = 0; i < 1000; ++i)
        {
            c += b;
            benchmark::DoNotOptimize(c);
        }
    }
}

static void BM_Wide256Subtraction(benchmark::State & state)
{
    WInt256 a = 1234567890;
    WInt256 b = 987654321;
    for (auto _ : state)
    {
        WInt256 c = a;
        for (int i = 0; i < 1000; ++i)
        {
            c -= b;
            benchmark::DoNotOptimize(c);
        }
    }
}

static void BM_Boost256Addition(benchmark::State & state)
{
    BInt256 a = 123456789;
    BInt256 b = 987654321;
    for (auto _ : state)
    {
        BInt256 c = a;
        for (int i = 0; i < 1000; ++i)
        {
            c += b;
            benchmark::DoNotOptimize(c);
        }
    }
}

static void BM_Boost256Subtraction(benchmark::State & state)
{
    BInt256 a = 1234567890;
    BInt256 b = 987654321;
    for (auto _ : state)
    {
        BInt256 c = a;
        for (int i = 0; i < 1000; ++i)
        {
            c -= b;
            benchmark::DoNotOptimize(c);
        }
    }
}

static void BM_Wide256Multiplication(benchmark::State & state)
{
    WInt256 a = 123456789;
    WInt256 b = 987654321;
    for (auto _ : state)
    {
        WInt256 c = a;
        for (int i = 0; i < 100; ++i)
        {
            c *= b;
            benchmark::DoNotOptimize(c);
        }
    }
}

static void BM_Wide256Division(benchmark::State & state)
{
    WInt256 a = WInt256{1234567890123456789ULL} << 128;
    WInt256 b = 987654321;
    for (auto _ : state)
    {
        WInt256 c = a;
        for (int i = 0; i < 100; ++i)
        {
            c /= b;
            benchmark::DoNotOptimize(c);
        }
    }
}

static void BM_Boost256Multiplication(benchmark::State & state)
{
    BInt256 a = 123456789;
    BInt256 b = 987654321;
    for (auto _ : state)
    {
        BInt256 c = a;
        for (int i = 0; i < 100; ++i)
        {
            c *= b;
            benchmark::DoNotOptimize(c);
        }
    }
}

static void BM_Boost256Division(benchmark::State & state)
{
    BInt256 a = BInt256{1234567890123456789ULL} << 128;
    BInt256 b = 987654321;
    for (auto _ : state)
    {
        BInt256 c = a;
        for (int i = 0; i < 100; ++i)
        {
            c /= b;
            benchmark::DoNotOptimize(c);
        }
    }
}

BENCHMARK(BM_Wide128Addition);
BENCHMARK(BM_Wide128Subtraction);
BENCHMARK(BM_Builtin128Addition);
BENCHMARK(BM_Builtin128Subtraction);
BENCHMARK(BM_Wide128Multiplication);
BENCHMARK(BM_Wide128Division);
BENCHMARK(BM_Builtin128Multiplication);
BENCHMARK(BM_Builtin128Division);
BENCHMARK(BM_Boost128Addition);
BENCHMARK(BM_Boost128Subtraction);
BENCHMARK(BM_Boost128Multiplication);
BENCHMARK(BM_Boost128Division);
BENCHMARK(BM_Wide256Addition);
BENCHMARK(BM_Wide256Subtraction);
BENCHMARK(BM_Boost256Addition);
BENCHMARK(BM_Boost256Subtraction);
BENCHMARK(BM_Wide256Multiplication);
BENCHMARK(BM_Wide256Division);
BENCHMARK(BM_Boost256Multiplication);
BENCHMARK(BM_Boost256Division);

BENCHMARK_MAIN();

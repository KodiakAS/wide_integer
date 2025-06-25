#include <benchmark/benchmark.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <wide_integer/wide_integer.h>

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

BENCHMARK(BM_Wide128Addition);
BENCHMARK(BM_Builtin128Addition);
BENCHMARK(BM_Wide128Multiplication);
BENCHMARK(BM_Builtin128Multiplication);
BENCHMARK(BM_Boost128Addition);
BENCHMARK(BM_Boost128Multiplication);
BENCHMARK(BM_Wide256Addition);
BENCHMARK(BM_Boost256Addition);
BENCHMARK(BM_Wide256Multiplication);
BENCHMARK(BM_Boost256Multiplication);

BENCHMARK_MAIN();

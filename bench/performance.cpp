#include <benchmark/benchmark.h>

#ifdef USE_CXX11_HEADER
#    include <wide_integer/wide_integer_cxx11.h>
#else
#    include <wide_integer/wide_integer.h>
#endif

using WInt = wide::integer<256, unsigned>;

static void BM_Addition(benchmark::State & state)
{
    WInt a = 123456789;
    WInt b = 987654321;
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(a += b);
    }
}

static void BM_Subtraction(benchmark::State & state)
{
    WInt a = 987654321;
    WInt b = 123456789;
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(a -= b);
    }
}

static void BM_Multiplication(benchmark::State & state)
{
    WInt a = 123456789;
    WInt b = 987654321;
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(a *= b);
    }
}

static void BM_Division(benchmark::State & state)
{
    WInt a = 987654321;
    WInt b = 123456;
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(a /= b);
    }
}

BENCHMARK(BM_Addition);
BENCHMARK(BM_Subtraction);
BENCHMARK(BM_Multiplication);
BENCHMARK(BM_Division);

static void BM_ToString(benchmark::State & state)
{
    WInt a = (WInt(1) << 255) + 123456789;
    for (auto _ : state)
    {
        auto s = wide::to_string(a);
        benchmark::DoNotOptimize(s);
        a += WInt{1};
    }
}

BENCHMARK(BM_ToString);

BENCHMARK_MAIN();

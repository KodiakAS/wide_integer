#include <wide_integer/std_compat.h>

#include <benchmark/benchmark.h>

#ifdef USE_CXX11_HEADER
#    include <wide_integer/wide_integer_cxx11.h>
#else
#    include <wide_integer/wide_integer.h>
#endif

template <size_t Bits>
using WInt = wide::integer<Bits, unsigned>;

template <size_t Bits>
static void BM_Addition(benchmark::State & state)
{
    WInt<Bits> a = 123456789;
    WInt<Bits> b = 987654321;
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(a += b);
    }
}

template <size_t Bits>
static void BM_Subtraction(benchmark::State & state)
{
    WInt<Bits> a = 987654321;
    WInt<Bits> b = 123456789;
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(a -= b);
    }
}

template <size_t Bits>
static void BM_Multiplication(benchmark::State & state)
{
    WInt<Bits> a = 123456789;
    WInt<Bits> b = 987654321;
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(a *= b);
    }
}

template <size_t Bits>
static void BM_Division(benchmark::State & state)
{
    WInt<Bits> a = 987654321;
    WInt<Bits> b = 123456;
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(a /= b);
    }
}

BENCHMARK_TEMPLATE(BM_Addition, 256);
BENCHMARK_TEMPLATE(BM_Addition, 512);
BENCHMARK_TEMPLATE(BM_Addition, 1024);
BENCHMARK_TEMPLATE(BM_Subtraction, 256);
BENCHMARK_TEMPLATE(BM_Subtraction, 512);
BENCHMARK_TEMPLATE(BM_Subtraction, 1024);
BENCHMARK_TEMPLATE(BM_Multiplication, 256);
BENCHMARK_TEMPLATE(BM_Multiplication, 512);
BENCHMARK_TEMPLATE(BM_Multiplication, 1024);
BENCHMARK_TEMPLATE(BM_Division, 256);
BENCHMARK_TEMPLATE(BM_Division, 512);
BENCHMARK_TEMPLATE(BM_Division, 1024);

template <size_t Bits>
static void BM_ToString(benchmark::State & state)
{
    WInt<Bits> a = (WInt<Bits>(1) << int(Bits - 1)) + 123456789;
    for (auto _ : state)
    {
        auto s = wide::to_string(a);
        benchmark::DoNotOptimize(s);
        a += WInt<Bits>{1};
    }
}

BENCHMARK_TEMPLATE(BM_ToString, 256);
BENCHMARK_TEMPLATE(BM_ToString, 512);
BENCHMARK_TEMPLATE(BM_ToString, 1024);

BENCHMARK_MAIN();

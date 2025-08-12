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
        WInt c = a + b;
        benchmark::DoNotOptimize(c);
    }
}

static void BM_Subtraction(benchmark::State & state)
{
    WInt a = 987654321;
    WInt b = 123456789;
    for (auto _ : state)
    {
        WInt c = a - b;
        benchmark::DoNotOptimize(c);
    }
}

static void BM_Multiplication(benchmark::State & state)
{
    WInt a = 123456789;
    WInt b = 987654321;
    for (auto _ : state)
    {
        WInt c = a * b;
        benchmark::DoNotOptimize(c);
    }
}

static void BM_Division(benchmark::State & state)
{
    WInt a = 987654321;
    WInt b = 123456;
    for (auto _ : state)
    {
        WInt c = a / b;
        benchmark::DoNotOptimize(c);
    }
}

BENCHMARK(BM_Addition);
BENCHMARK(BM_Subtraction);
BENCHMARK(BM_Multiplication);
BENCHMARK(BM_Division);

static std::string to_string_slow(WInt n)
{
    std::string res;
    if (WInt::_impl::operator_eq(n, 0U))
        return "0";
    while (!WInt::_impl::operator_eq(n, 0U))
    {
        res.insert(res.begin(), '0' + char(WInt::_impl::operator_percent(n, 10U)));
        n = WInt::_impl::operator_slash(n, 10U);
    }
    return res;
}

static void BM_ToStringOld(benchmark::State & state)
{
    WInt a = (WInt(1) << 255) + 123456789;
    for (auto _ : state)
    {
        auto s = to_string_slow(a);
        benchmark::DoNotOptimize(s);
    }
}

static void BM_ToStringNew(benchmark::State & state)
{
    WInt a = (WInt(1) << 255) + 123456789;
    for (auto _ : state)
    {
        auto s = wide::to_string(a);
        benchmark::DoNotOptimize(s);
    }
}

BENCHMARK(BM_ToStringOld);
BENCHMARK(BM_ToStringNew);

BENCHMARK_MAIN();

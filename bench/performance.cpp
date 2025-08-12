#include <benchmark/benchmark.h>
#include <wide_integer/wide_integer.h>

using WInt = wide::integer<256, unsigned>;

static void BM_Addition(benchmark::State & state)
{
    WInt a = 123456789;
    WInt b = 987654321;
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(a);
        benchmark::DoNotOptimize(b);
        auto c = a + b;
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
    }
}

static void BM_Subtraction(benchmark::State & state)
{
    WInt a = 987654321;
    WInt b = 123456789;
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(a);
        benchmark::DoNotOptimize(b);
        auto c = a - b;
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
    }
}

static void BM_Multiplication(benchmark::State & state)
{
    WInt a = 123456789;
    WInt b = 987654321;
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(a);
        benchmark::DoNotOptimize(b);
        auto c = a * b;
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
    }
}

static void BM_Division(benchmark::State & state)
{
    WInt a = 987654321;
    WInt b = 123456;
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(a);
        benchmark::DoNotOptimize(b);
        auto c = a / b;
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
    }
}

BENCHMARK(BM_Addition);
BENCHMARK(BM_Subtraction);
BENCHMARK(BM_Multiplication);
BENCHMARK(BM_Division);

static std::string to_string_slow(WInt n)
{
    std::string res;
    if (n == 0)
        return "0";
    const WInt ten = 10;
    while (n != 0)
    {
        auto digit = static_cast<unsigned>(n % ten);
        res.insert(res.begin(), char('0' + digit));
        n /= ten;
    }
    return res;
}

static void BM_ToStringOld(benchmark::State & state)
{
    WInt a = (WInt(1) << 255) + 123456789;
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(a);
        auto s = to_string_slow(a);
        benchmark::DoNotOptimize(s);
        benchmark::ClobberMemory();
    }
}

static void BM_ToStringNew(benchmark::State & state)
{
    WInt a = (WInt(1) << 255) + 123456789;
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(a);
        auto s = wide::to_string(a);
        benchmark::DoNotOptimize(s);
        benchmark::ClobberMemory();
    }
}

BENCHMARK(BM_ToStringOld);
BENCHMARK(BM_ToStringNew);

BENCHMARK_MAIN();

# wide_integer
wide integer implementation in CH

## Performance

Benchmarks comparing C++11 and C++17 builds of `wide_integer` with
Boost.Multiprecision (`bench/compare_int128.cpp`) produce the following timings
on a 2.8 GHz CPU (lower is better):

| Operation               | C++11 (`wide_integer`) | C++17 (`wide_integer`) | Boost.Multiprecision |
| ----------------------- | ---------------------: | ---------------------: | -------------------: |
| 128‑bit addition        |                 1965 ns |                 2040 ns |             2335 ns |
| 128‑bit subtraction     |                 2053 ns |                 2198 ns |             2324 ns |
| 128‑bit multiplication  |                  241 ns |                  227 ns |              232 ns |
| 128‑bit division        |                  494 ns |                  537 ns |              528 ns |
| 256‑bit addition        |                 1993 ns |                 1848 ns |             2074 ns |
| 256‑bit subtraction     |                 2410 ns |                 1947 ns |             4019 ns |
| 256‑bit multiplication  |                  350 ns |                  361 ns |              586 ns |
| 256‑bit division        |                 4099 ns |                 4114 ns |             1282 ns |

The looped benchmark highlights that the library provides competitive
performance against Boost, particularly for 256‑bit multiplication.

## Quick Start

```cpp
#include <wide_integer/wide_integer.h>

int main() {
    wide::integer<256, unsigned> a = 1;
    wide::integer<256, unsigned> b = 2;
    auto c = (a << 128) + b;
    auto low = static_cast<std::uint64_t>(c); // low == 2
    (void)low;
}
```

## Building Tests

```bash
cmake -S . -B build
cmake --build build
cd build && ctest
```

## Building Benchmarks

```bash
cmake -S . -B build
cmake --build build --target perf_cxx17 perf_cxx11 perf_compare_int128 perf_compare_int128_cxx11
```

## Code Coverage

To generate coverage locally, configure the project with the `ENABLE_COVERAGE` option
and build the tests:

```bash
cmake -S . -B build -DENABLE_COVERAGE=ON
cmake --build build --config Debug
cd build && ctest
```

After running the tests, coverage information can be collected with `lcov`:

```bash
lcov --capture --directory build --output-file coverage.info --ignore-errors mismatch
lcov --remove coverage.info '/usr/*' '*/tests/*' --output-file coverage.info
lcov --list coverage.info
```

Note that function coverage may show as `0%` because this library is header-only
and templates are instantiated in the test binaries.

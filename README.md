# wide_integer
wide integer implementation in CH

## Performance

Benchmarks comparing the C++17 implementation against Boost.Multiprecision
(`bench/compare_int128.cpp`) show that `wide_integer` performs operations
efficiently. Sample results measured on a 2.8 GHz CPU are:

| Operation              | `wide_integer` | Boost.Multiprecision |
| ---------------------- | -------------: | -------------------: |
| 128‑bit addition       |        2004 ns |             2270 ns |
| 128‑bit multiplication |         223 ns |              220 ns |
| 256‑bit addition       |        1751 ns |             1864 ns |
| 256‑bit multiplication |         370 ns |              519 ns |

`wide_integer` is ~12% faster for 128‑bit addition and ~29% faster for 256‑bit
multiplication in this benchmark, while other operations are on par with the
Boost implementation.

## Quick Start

```cpp
#include <wide_integer/wide_integer.h>

int main() {
    wide::integer<256, unsigned> value = 1;
    value <<= 128;
    auto s = wide::to_string(value);
    // use s...
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
cmake --build build --target perf_cxx17 perf_cxx11 perf_compare_int128
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

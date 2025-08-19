# wide_integer
An independent, header-only C++ library built on ClickHouse's wide integer work to implement high-performance fixed-width integers.

- Provides separate C++11 and C++17 implementations to fit different project requirements
- Guarantees that bit widths exactly match their definition (e.g., `wide::integer<256, unsigned>` is exactly 256 bits)
- Supports arithmetic operations and conversions with standard C++ integer types
- Depends only on `fmt` for formatted output

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
make test
```

## Building Benchmarks

```bash
make bench
```

## Code Coverage

To generate a coverage report locally:

```bash
make coverage
```

This builds the project with coverage flags, runs the tests, and
produces a coverage report under the `build-coverage` directory.

Note that function coverage may show as `0%` because this library is header-only
and templates are instantiated in the test binaries.

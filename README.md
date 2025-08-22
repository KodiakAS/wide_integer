# wide_integer
An independent, header-only C++ library built on ClickHouse's wide integer work to implement high-performance fixed-width integers.

- Provides separate C++11 and C++17 implementations to fit different project requirements
- Guarantees that bit widths exactly match their definition (e.g., `wide::integer<256, unsigned>` is exactly 256 bits)
- Supports arithmetic operations and conversions with standard C++ integer types
- Depends only on `fmt` for formatted output

## Performance

Benchmarks on a 2.8 GHz CPU compare the C++11 implementation of
`wide::integer<256>` against Boost.Multiprecision's `int256_t`. Timings
are in nanoseconds (lower is better).

| Operation (mixed operands) | `wide_integer` | Boost.Multiprecision |
| ------------------------- | -------------: | -------------------: |
| Addition                  |        3.70 ns |             10.4 ns |
| Subtraction               |        4.57 ns |             25.4 ns |
| Multiplication            |        9.42 ns |             22.3 ns |
| Division                  |        21.0 ns |              161 ns |

`wide_integer` outperforms Boost for most 256‑bit operations; Boost can
still be faster when operands are very small, particularly for
multiplication.

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

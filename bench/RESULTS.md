# Performance comparison

This benchmark exercises the C++11 implementation of `wide::integer<256, signed>` against Boost.Multiprecision's `int256_t` for a variety of operand sizes and sign combinations.

To run:
```bash
cmake -S . -B build-release-bench -DWI_BUILD_TESTS=OFF -DWI_BUILD_BENCHMARKS=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build-release-bench --config Release -j$(nproc)
./build-release-bench/perf_compare_int256_cxx11 --benchmark_min_time=0.01s
```

Sample output on a 2.8 GHz CPU:
```text
Add/Mixed/Wide        3.70 ns         3.71 ns      4096975
Add/Mixed/Boost       10.4 ns         10.4 ns      1000000
Sub/Small/Wide        12.0 ns         12.0 ns      2229799
Sub/Small/Boost       3.89 ns         3.89 ns      3898244
Sub/Large/Wide        4.48 ns         4.48 ns      2986277
Sub/Large/Boost       10.8 ns         10.8 ns      1249906
Sub/Mixed/Wide        4.57 ns         4.57 ns      3141281
Sub/Mixed/Boost       25.4 ns         25.4 ns      1000000
Mul/Small/Wide        5.56 ns         5.56 ns      2483987
Mul/Small/Boost       4.78 ns         4.78 ns      2772797
Mul/Large/Wide        10.3 ns         10.3 ns      1288206
Mul/Large/Boost       21.0 ns         21.0 ns       541291
Mul/Mixed/Wide        9.42 ns         9.42 ns      1343267
Mul/Mixed/Boost       22.3 ns         22.3 ns       622942
Div/Small/Wide        16.4 ns         16.4 ns       864976
Div/Small/Boost       34.9 ns         34.9 ns       361811
Div/Large/Wide         125 ns          126 ns       121522
Div/Large/Boost        166 ns          166 ns        84391
Div/Mixed/Wide        21.0 ns         21.0 ns       655858
Div/Mixed/Boost        161 ns          161 ns        87062
```

`wide_integer` is faster than Boost for most large or mixed 256‑bit operations, while Boost can still win on small-input multiplication and subtraction.

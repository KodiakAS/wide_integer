# Performance comparison

This benchmark compares `wide::integer<128>` against GCC's built-in `__int128` and Boost's `uint128_t`. It also
compares `wide::integer<256>` with Boost's `uint256_t`.

To run:
```bash
cmake -S . -B build
cmake --build build --config Release -j$(nproc)
./build/perf_compare_int128
```

Sample output on a 2.8 GHz CPU:
```
Benchmark                            Time             CPU   Iterations
BM_Wide128Addition                2004 ns         2004 ns         6991
BM_Builtin128Addition             2261 ns         2261 ns         6085
BM_Wide128Multiplication           223 ns          223 ns        62189
BM_Builtin128Multiplication        218 ns          218 ns        63801
BM_Boost128Addition               2270 ns         2270 ns         6193
BM_Boost128Multiplication          220 ns          220 ns        63845
BM_Wide256Addition                1751 ns         1751 ns         7989
BM_Boost256Addition               1864 ns         1864 ns         7588
BM_Wide256Multiplication           370 ns          372 ns        42234
BM_Boost256Multiplication          519 ns          519 ns        26950
```

The looped benchmark makes timing differences more visible. `wide_integer` has
slightly faster additions than the compiler builtin or Boost types, while its
256‑bit multiplication remains faster than Boost's implementation.

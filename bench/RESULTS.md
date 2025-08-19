# Performance comparison

This benchmark compares `wide::integer<128>` against GCC's built-in `__int128` and Boost's `uint128_t`. It also
compares `wide::integer<256>` with Boost's `uint256_t`.

To run:
```bash
cmake -S . -B build
cmake --build build --config Release -j$(nproc)
./build/perf_compare_int128
./build/perf_compare_int128_cxx11
```

Sample output on a 2.8 GHz CPU:
```
Benchmark                            Time             CPU   Iterations
BM_Wide128Addition                2040 ns         2038 ns       335721
BM_Wide128Subtraction             2198 ns         2196 ns       319258
BM_Builtin128Addition             2335 ns         2333 ns       302507
BM_Builtin128Subtraction          2306 ns         2304 ns       302659
BM_Wide128Multiplication           227 ns          227 ns      3106920
BM_Wide128Division                 537 ns          536 ns      1305691
BM_Builtin128Multiplication        232 ns          232 ns      3055719
BM_Builtin128Division              539 ns          539 ns      1226872
BM_Boost128Addition               2335 ns         2334 ns       302943
BM_Boost128Subtraction            2324 ns         2323 ns       300175
BM_Boost128Multiplication          232 ns          232 ns      3023021
BM_Boost128Division                528 ns          528 ns      1000000
BM_Wide256Addition                1848 ns         1847 ns       380149
BM_Wide256Subtraction             1947 ns         1947 ns       344306
BM_Boost256Addition               2074 ns         2072 ns       351470
BM_Boost256Subtraction            4019 ns         4019 ns       165910
BM_Wide256Multiplication           361 ns          361 ns      1944052
BM_Wide256Division                4114 ns         4113 ns       136049
BM_Boost256Multiplication          586 ns          586 ns      1202912
BM_Boost256Division               1282 ns         1282 ns       500483
```

Sample output for the C++11 implementation:
```
Benchmark                            Time             CPU   Iterations
BM_Wide128Addition                1965 ns         1965 ns       316455
BM_Wide128Subtraction             2053 ns         2053 ns       345626
BM_Builtin128Addition             2314 ns         2314 ns       292793
BM_Builtin128Subtraction          2301 ns         2301 ns       303393
BM_Wide128Multiplication           241 ns          241 ns      2907557
BM_Wide128Division                 494 ns          494 ns      1290217
BM_Builtin128Multiplication        233 ns          233 ns      3071131
BM_Builtin128Division              568 ns          568 ns      1232465
BM_Boost128Addition               2328 ns         2328 ns       300796
BM_Boost128Subtraction            2326 ns         2326 ns       301175
BM_Boost128Multiplication          232 ns          232 ns      3003255
BM_Boost128Division                544 ns          544 ns      1285022
BM_Wide256Addition                1993 ns         1993 ns       344203
BM_Wide256Subtraction             2410 ns         2410 ns       319426
BM_Boost256Addition               2059 ns         2058 ns       341676
BM_Boost256Subtraction            4385 ns         4385 ns       163912
BM_Wide256Multiplication           350 ns          350 ns      2004604
BM_Wide256Division                4099 ns         4099 ns       162684
BM_Boost256Multiplication          561 ns          561 ns      1309018
BM_Boost256Division               1293 ns         1293 ns       561343
```

The looped benchmark makes timing differences more visible. `wide_integer` has
slightly faster additions than the compiler builtin or Boost types, while its
256‑bit multiplication remains faster than Boost's implementation.


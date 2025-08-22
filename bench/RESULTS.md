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

## C++11 `int256` vs Boost

This benchmark compares the C++11 implementation of `wide::integer<256, signed>`
against Boost's `int256_t` for a variety of operand sizes and signs.

To run:
```bash
cmake -S . -B build
cmake --build build --config Release -j$(nproc)
./build/perf_compare_int256_cxx11 --benchmark_min_time=0.01s
```

Sample output:
```
----------------------------------------------------------
Benchmark                Time             CPU   Iterations
----------------------------------------------------------
Add/Small/Wide       0.594 ns        0.594 ns     23639169
Add/Small/Boost       2.51 ns         2.51 ns      5522541
Add/Large/Wide       0.587 ns        0.587 ns     23868318
Add/Large/Boost       5.62 ns         5.62 ns      2542542
Add/Mixed/Wide        1.18 ns         1.18 ns     11497321
Add/Mixed/Boost       5.66 ns         5.66 ns      2500564
Sub/Small/Wide       0.586 ns        0.586 ns     23669159
Sub/Small/Boost       2.08 ns         2.08 ns      6677669
Sub/Large/Wide       0.589 ns        0.589 ns     23724346
Sub/Large/Boost       5.95 ns         5.95 ns      2312915
Sub/Mixed/Wide       0.592 ns        0.592 ns     23616654
Sub/Mixed/Boost       5.57 ns         5.57 ns      2502206
Mul/Small/Wide       0.585 ns        0.585 ns     23964429
Mul/Small/Boost       2.09 ns         2.09 ns      6831201
Mul/Large/Wide       0.600 ns        0.600 ns     23625315
Mul/Large/Boost       14.0 ns         14.0 ns       973061
Mul/Mixed/Wide        2.01 ns         2.01 ns      6999199
Mul/Mixed/Boost       15.1 ns         15.1 ns       967657
Div/Small/Wide        14.6 ns         14.6 ns       957805
Div/Small/Boost       10.1 ns         10.1 ns      1464733
Div/Large/Wide         620 ns          620 ns        22840
Div/Large/Boost       56.6 ns         56.6 ns       247870
Div/Mixed/Wide         634 ns          634 ns        22996
Div/Mixed/Boost       62.3 ns         62.4 ns       230513
```


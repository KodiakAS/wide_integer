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

## C++11 vs C++17

Benchmarking the same operations under different C++ standards shows comparable performance. Sample results from the `performance.cpp` benchmark:

### C++17
```
BM_Addition             1.62 ns         1.62 ns      7709981
BM_Subtraction          2.06 ns         2.06 ns      6982822
BM_Multiplication       8.09 ns         8.09 ns      1729611
BM_Division             99.6 ns         99.6 ns       137063
BM_ToStringOld        179251 ns       179247 ns           76
BM_ToStringNew         97955 ns        97953 ns          135
```

### C++11
```
BM_Addition             1.78 ns         1.78 ns      7415415
BM_Subtraction          2.03 ns         2.03 ns      6982707
BM_Multiplication       8.12 ns         8.12 ns      1722390
BM_Division             97.7 ns         97.7 ns       147232
BM_ToStringOld        185689 ns       185693 ns           76
BM_ToStringNew         98485 ns        98481 ns          140
```

| Benchmark      | C++17 (ns) | C++11 (ns) | C++11/17 |
|---------------|-----------:|-----------:|---------:|
| Addition       |       1.62 |       1.78 |    1.10x |
| Subtraction    |       2.06 |       2.03 |    0.99x |
| Multiplication |       8.09 |       8.12 |    1.00x |
| Division       |       99.6 |       97.7 |    0.98x |
| ToStringOld    |    179251  |    185689  |    1.04x |
| ToStringNew    |     97955  |     98485  |    1.01x |

Arithmetic operations are within the same order of magnitude, with timing differences generally under 10%.

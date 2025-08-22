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
BM_Wide128Addition                2119 ns         2119 ns       335094
BM_Wide128Subtraction             2190 ns         2190 ns       322166
BM_Builtin128Addition             2286 ns         2286 ns       307653
BM_Builtin128Subtraction          2283 ns         2283 ns       306640
BM_Wide128Multiplication           230 ns          230 ns      3061851
BM_Wide128Division                 541 ns          541 ns      1286217
BM_Builtin128Multiplication        226 ns          226 ns      3096622
BM_Builtin128Division              560 ns          560 ns      1228240
BM_Boost128Addition               2305 ns         2305 ns       305968
BM_Boost128Subtraction            2302 ns         2302 ns       303559
BM_Boost128Multiplication          223 ns          223 ns      3082484
BM_Boost128Division                573 ns          573 ns      1222177
BM_Wide256Addition                1807 ns         1807 ns       392372
BM_Wide256Subtraction             1873 ns         1873 ns       375089
BM_Boost256Addition               1925 ns         1925 ns       370704
BM_Boost256Subtraction            3911 ns         3911 ns       185063
BM_Wide256Multiplication           325 ns          325 ns      2160624
BM_Wide256Division                1489 ns         1489 ns       474118
BM_Boost256Multiplication          549 ns          549 ns      1300117
BM_Boost256Division               1178 ns         1178 ns       598816
```

Sample output for the C++11 implementation:
```
Benchmark                            Time             CPU   Iterations
BM_Wide128Addition                1837 ns         1837 ns       384807
BM_Wide128Subtraction             4499 ns         4499 ns       151749
BM_Builtin128Addition             2306 ns         2306 ns       300732
BM_Builtin128Subtraction          2326 ns         2326 ns       302106
BM_Wide128Multiplication           242 ns          242 ns      2955489
BM_Wide128Division                 554 ns          554 ns      1262717
BM_Builtin128Multiplication        229 ns          229 ns      3084519
BM_Builtin128Division              571 ns          571 ns      1230586
BM_Boost128Addition               2311 ns         2311 ns       301522
BM_Boost128Subtraction            2311 ns         2311 ns       305205
BM_Boost128Multiplication          227 ns          227 ns      3115710
BM_Boost128Division                573 ns          573 ns      1227688
BM_Wide256Addition                1902 ns         1902 ns       369321
BM_Wide256Subtraction             2109 ns         2109 ns       333797
BM_Boost256Addition               1948 ns         1948 ns       365818
BM_Boost256Subtraction            4138 ns         4138 ns       165978
BM_Wide256Multiplication           345 ns          345 ns      2039112
BM_Wide256Division                1021 ns         1021 ns       673262
BM_Boost256Multiplication          523 ns          523 ns      1348030
BM_Boost256Division               1266 ns         1266 ns       588112
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
Add/Small/Wide        1.88 ns         1.88 ns      7301764
Add/Small/Boost       2.30 ns         2.29 ns      6055725
Add/Large/Wide        2.10 ns         2.10 ns      7171356
Add/Large/Boost       6.84 ns         6.84 ns      2422954
Add/Mixed/Wide        1.78 ns         1.78 ns      7806488
Add/Mixed/Boost       5.76 ns         5.76 ns      2371206
Sub/Small/Wide        2.81 ns         2.81 ns      5099976
Sub/Small/Boost       2.89 ns         2.89 ns      4832640
Sub/Large/Wide        2.81 ns         2.81 ns      4926732
Sub/Large/Boost       6.57 ns         6.57 ns      2272943
Sub/Mixed/Wide        2.74 ns         2.74 ns      5297833
Sub/Mixed/Boost       4.99 ns         4.99 ns      2811051
Mul/Small/Wide        3.32 ns         3.32 ns      4502986
Mul/Small/Boost       2.77 ns         2.77 ns      5896269
Mul/Large/Wide        6.14 ns         6.14 ns      2135381
Mul/Large/Boost       12.9 ns         12.9 ns      1105486
Mul/Mixed/Wide        5.77 ns         5.77 ns      2464757
Mul/Mixed/Boost       14.8 ns         14.8 ns       946250
Div/Small/Wide        5.48 ns         5.49 ns      2536622
Div/Small/Boost       9.94 ns         9.94 ns      1329075
Div/Large/Wide       41.2 ns         41.2 ns       339445
Div/Large/Boost       54.8 ns         54.8 ns       245143
Div/Mixed/Wide       20.5 ns         20.5 ns       678880
Div/Mixed/Boost       60.4 ns         60.4 ns       236945
```


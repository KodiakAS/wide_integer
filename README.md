# wide_integer
wide integer implementation in CH

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
lcov --capture --directory build --output-file coverage.info
lcov --remove coverage.info '/usr/*' --output-file coverage.info
lcov --list coverage.info
```

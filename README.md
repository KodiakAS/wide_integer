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
lcov --capture --directory build --output-file coverage.info --ignore-errors mismatch
lcov --remove coverage.info '/usr/*' '*/tests/*' --output-file coverage.info
lcov --list coverage.info
```

Note that function coverage may show as `0%` because this library is header-only
and templates are instantiated in the test binaries.

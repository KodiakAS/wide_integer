TEST_BUILD_DIR ?= build
BENCH_BUILD_DIR ?= build-bench
COVERAGE_DIR ?= build-coverage

.PHONY: test bench coverage clean

$(TEST_BUILD_DIR)/Makefile:
	cmake -S . -B $(TEST_BUILD_DIR) -DWI_BUILD_TESTS=ON -DWI_BUILD_BENCHMARKS=OFF

$(BENCH_BUILD_DIR)/Makefile:
	cmake -S . -B $(BENCH_BUILD_DIR) -DWI_BUILD_TESTS=OFF -DWI_BUILD_BENCHMARKS=ON

$(COVERAGE_DIR)/Makefile:
	cmake -S . -B $(COVERAGE_DIR) -DENABLE_COVERAGE=ON -DWI_BUILD_TESTS=ON -DWI_BUILD_BENCHMARKS=OFF

# Build and run unit tests
test: $(TEST_BUILD_DIR)/Makefile
	cmake --build $(TEST_BUILD_DIR)
	cd $(TEST_BUILD_DIR) && ctest --output-on-failure

# Build and run benchmarks
bench: $(BENCH_BUILD_DIR)/Makefile
	cmake --build $(BENCH_BUILD_DIR)
	$(BENCH_BUILD_DIR)/perf
	$(BENCH_BUILD_DIR)/perf_compare_int256

# Build, test and generate coverage report
coverage: $(COVERAGE_DIR)/Makefile
	cmake --build $(COVERAGE_DIR) --config Debug
	cd $(COVERAGE_DIR) && ctest --output-on-failure
	lcov --capture --directory $(COVERAGE_DIR) --output-file $(COVERAGE_DIR)/coverage.info --ignore-errors mismatch
	lcov --remove $(COVERAGE_DIR)/coverage.info '/usr/*' '*/tests/*' --output-file $(COVERAGE_DIR)/coverage.info
	lcov --list $(COVERAGE_DIR)/coverage.info

# Remove build directories
clean:
	rm -rf $(TEST_BUILD_DIR) $(BENCH_BUILD_DIR) $(COVERAGE_DIR)

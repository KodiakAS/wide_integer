BUILD_DIR ?= build
COVERAGE_DIR ?= build-coverage

.PHONY: test bench coverage clean

$(BUILD_DIR)/Makefile:
	cmake -S . -B $(BUILD_DIR)

$(COVERAGE_DIR)/Makefile:
	cmake -S . -B $(COVERAGE_DIR) -DENABLE_COVERAGE=ON

# Build and run unit tests
test: $(BUILD_DIR)/Makefile
	cmake --build $(BUILD_DIR)
	cd $(BUILD_DIR) && ctest --output-on-failure

# Build and run benchmarks
bench: $(BUILD_DIR)/Makefile
	cmake --build $(BUILD_DIR) --target perf_cxx17 perf_cxx11 perf_compare_int128 perf_compare_int128_cxx11 perf_compare_int256_cxx11
	$(BUILD_DIR)/perf_cxx17
	$(BUILD_DIR)/perf_cxx11
	$(BUILD_DIR)/perf_compare_int128
	$(BUILD_DIR)/perf_compare_int128_cxx11
	$(BUILD_DIR)/perf_compare_int256_cxx11

# Build, test and generate coverage report
coverage: $(COVERAGE_DIR)/Makefile
	cmake --build $(COVERAGE_DIR) --config Debug
	cd $(COVERAGE_DIR) && ctest --output-on-failure
	lcov --capture --directory $(COVERAGE_DIR) --output-file $(COVERAGE_DIR)/coverage.info --ignore-errors mismatch
	lcov --remove $(COVERAGE_DIR)/coverage.info '/usr/*' '*/tests/*' --output-file $(COVERAGE_DIR)/coverage.info
	lcov --list $(COVERAGE_DIR)/coverage.info

# Remove build directories
clean:
	rm -rf $(BUILD_DIR) $(COVERAGE_DIR)

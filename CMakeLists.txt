cmake_minimum_required(VERSION 3.10)
project(wide_integer LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

option(ENABLE_COVERAGE "Enable coverage reporting" OFF)
option(WI_BUILD_TESTS "Build test suite" ON)
option(WI_BUILD_BENCHMARKS "Build benchmarks" OFF)

set(WI_TEST_OPTIONS -O0 -g)
set(WI_TEST_LINK_OPTIONS "")

if(ENABLE_COVERAGE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        list(APPEND WI_TEST_OPTIONS --coverage)
        set(WI_TEST_LINK_OPTIONS --coverage)
    endif()
endif()

find_package(fmt CONFIG QUIET)
if(NOT fmt_FOUND)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(fmt REQUIRED IMPORTED_TARGET fmt)
    add_library(fmt::fmt ALIAS PkgConfig::fmt)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

add_library(wide_integer INTERFACE)

target_include_directories(wide_integer INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(wide_integer INTERFACE cxx_std_17)

if(WI_BUILD_TESTS)
    find_package(GTest REQUIRED)
    enable_testing()
    include(GoogleTest)
    function(add_wide_test name source std)
        add_executable(${name} ${source})
        target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
        set_target_properties(${name} PROPERTIES CXX_STANDARD ${std} CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS YES)
        target_compile_options(${name} PRIVATE ${WI_TEST_OPTIONS})
        if(WI_TEST_LINK_OPTIONS)
            target_link_options(${name} PRIVATE ${WI_TEST_LINK_OPTIONS})
        endif()
        target_link_libraries(${name} PRIVATE GTest::gtest_main)
        gtest_discover_tests(${name} NO_PRETTY_VALUES)
    endfunction()

    add_wide_test(wide_integer_test tests/wide_integer_test.cpp 17)
    target_link_libraries(wide_integer_test PRIVATE fmt::fmt)
    add_wide_test(wide_integer_cxx11_test tests/wide_integer_test.cpp 11)
    target_compile_definitions(wide_integer_cxx11_test PRIVATE USE_CXX11_HEADER)
    target_link_libraries(wide_integer_cxx11_test PRIVATE fmt::fmt)
endif()

if(WI_BUILD_BENCHMARKS)
    find_package(benchmark REQUIRED)

    add_executable(perf_cxx17
        bench/performance.cpp
    )
    target_include_directories(perf_cxx17 PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    target_compile_features(perf_cxx17 PRIVATE cxx_std_17)
    target_link_libraries(perf_cxx17 PRIVATE fmt::fmt benchmark::benchmark)
    target_compile_options(perf_cxx17 PRIVATE -O3 -DNDEBUG)

    add_executable(perf_cxx11
        bench/performance.cpp
    )
    target_compile_definitions(perf_cxx11 PRIVATE USE_CXX11_HEADER)
    target_include_directories(perf_cxx11 PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    target_compile_features(perf_cxx11 PRIVATE cxx_std_11)
    target_link_libraries(perf_cxx11 PRIVATE fmt::fmt benchmark::benchmark)
    target_compile_options(perf_cxx11 PRIVATE -O3 -DNDEBUG)

    add_executable(perf_compare_int128
        bench/compare_int128.cpp
    )
    target_compile_features(perf_compare_int128 PRIVATE cxx_std_17)
    target_include_directories(perf_compare_int128 PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    target_link_libraries(perf_compare_int128 PRIVATE wide_integer fmt::fmt benchmark::benchmark)
    target_compile_options(perf_compare_int128 PRIVATE -O3 -DNDEBUG)

    add_executable(perf_compare_int128_cxx11
        bench/compare_int128.cpp
    )
    target_compile_definitions(perf_compare_int128_cxx11 PRIVATE USE_CXX11_HEADER)
    target_compile_features(perf_compare_int128_cxx11 PRIVATE cxx_std_11)
    target_include_directories(perf_compare_int128_cxx11 PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    target_link_libraries(perf_compare_int128_cxx11 PRIVATE fmt::fmt benchmark::benchmark)
    target_compile_options(perf_compare_int128_cxx11 PRIVATE -O3 -DNDEBUG)

    add_executable(perf_compare_int256_cxx11
        bench/compare_int256.cpp
    )
    target_compile_definitions(perf_compare_int256_cxx11 PRIVATE USE_CXX11_HEADER)
    target_compile_features(perf_compare_int256_cxx11 PRIVATE cxx_std_11)
    target_include_directories(perf_compare_int256_cxx11 PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    target_link_libraries(perf_compare_int256_cxx11 PRIVATE fmt::fmt benchmark::benchmark)
    target_compile_options(perf_compare_int256_cxx11 PRIVATE -O3 -DNDEBUG)

    add_executable(perf_compare_int256_random_cxx11
        bench/compare_int256_random.cpp
    )
    target_compile_definitions(perf_compare_int256_random_cxx11 PRIVATE USE_CXX11_HEADER)
    target_compile_features(perf_compare_int256_random_cxx11 PRIVATE cxx_std_11)
    target_include_directories(perf_compare_int256_random_cxx11 PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    target_link_libraries(perf_compare_int256_random_cxx11 PRIVATE fmt::fmt benchmark::benchmark)
    target_compile_options(perf_compare_int256_random_cxx11 PRIVATE -O3 -DNDEBUG)
endif()

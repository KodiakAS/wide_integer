cmake_minimum_required(VERSION 3.10)
project(wide_integer LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

option(ENABLE_COVERAGE "Enable coverage reporting" OFF)

set(WI_TEST_OPTIONS -O0 -g)
set(WI_TEST_LINK_OPTIONS "")

if(ENABLE_COVERAGE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        list(APPEND WI_TEST_OPTIONS --coverage)
        set(WI_TEST_LINK_OPTIONS --coverage)
    endif()
endif()

find_package(fmt REQUIRED)
find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

add_library(wide_integer INTERFACE)

target_include_directories(wide_integer INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(wide_integer INTERFACE cxx_std_17)

enable_testing()
include(GoogleTest)
function(add_wide_test name source std)
    add_executable(${name} ${source})
    target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_compile_features(${name} PRIVATE cxx_std_${std})
    target_compile_options(${name} PRIVATE ${WI_TEST_OPTIONS})
    if(WI_TEST_LINK_OPTIONS)
        target_link_options(${name} PRIVATE ${WI_TEST_LINK_OPTIONS})
    endif()
    target_link_libraries(${name} PRIVATE wide_integer fmt::fmt GTest::gtest_main)
    gtest_discover_tests(${name})
endfunction()

add_wide_test(wide_integer_test tests/wide_integer_test.cpp 17)
add_wide_test(wide_integer_cxx11_test tests/wide_integer_cxx11_test.cpp 11)


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

cmake_minimum_required(VERSION 3.10)
project(wide_integer LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

find_package(fmt REQUIRED)
find_package(GTest REQUIRED)

add_library(wide_integer INTERFACE)

target_include_directories(wide_integer INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(wide_integer INTERFACE cxx_std_17)

enable_testing()
include(GoogleTest)
add_executable(wide_integer_test
    tests/wide_integer_test.cpp
)

target_compile_features(wide_integer_test PRIVATE cxx_std_17)

target_link_libraries(wide_integer_test PRIVATE wide_integer fmt::fmt GTest::gtest_main)

gtest_discover_tests(wide_integer_test)

add_executable(wide_integer_cxx11_test
    tests/wide_integer_cxx11_test.cpp
)
target_include_directories(wide_integer_cxx11_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(wide_integer_cxx11_test PRIVATE cxx_std_11)
target_link_libraries(wide_integer_cxx11_test PRIVATE fmt::fmt GTest::gtest_main)
gtest_discover_tests(wide_integer_cxx11_test)

add_executable(perf_cxx17
    bench/performance.cpp
)
target_include_directories(perf_cxx17 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(perf_cxx17 PRIVATE cxx_std_17)
target_link_libraries(perf_cxx17 PRIVATE fmt::fmt)

add_executable(perf_cxx20
    bench/performance.cpp
)
target_include_directories(perf_cxx20 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(perf_cxx20 PRIVATE cxx_std_20)
target_link_libraries(perf_cxx20 PRIVATE fmt::fmt)

add_executable(perf_cxx11
    bench/performance.cpp
)
target_compile_definitions(perf_cxx11 PRIVATE USE_CXX11_HEADER)
target_include_directories(perf_cxx11 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(perf_cxx11 PRIVATE cxx_std_11)
target_link_libraries(perf_cxx11 PRIVATE fmt::fmt)

cmake_minimum_required(VERSION 3.10)
project(wide_integer LANGUAGES CXX)

find_package(fmt REQUIRED)
find_package(GTest REQUIRED)

add_library(wide_integer INTERFACE)

target_include_directories(wide_integer INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(wide_integer INTERFACE cxx_std_20)

enable_testing()
include(GoogleTest)
add_executable(wide_integer_test
    tests/wide_integer_test.cpp
)

target_link_libraries(wide_integer_test PRIVATE wide_integer fmt::fmt GTest::gtest_main)

gtest_discover_tests(wide_integer_test)

cmake_minimum_required(VERSION 3.14)
project(MAI_OOP_2)

set(CMAKE_CXX_STANDART 17)
set(CMAKE_CXX_STANDART_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_library(array lib/array/array.hpp lib/array/array.cpp)
target_include_directories(array PUBLIC lib/array)

add_library(bit_string lib/bit_string/bit_string.hpp lib/bit_string/bit_string.cpp)
target_include_directories(bit_string PUBLIC lib/bit_string lib/array)
target_link_libraries(bit_string array)

add_executable(array_unit tests/array/unit.cpp)
target_link_libraries(array_unit array gtest_main)

add_executable(bit_string_unit tests/bit_string/unit.cpp)
target_link_libraries(bit_string_unit bit_string gtest_main)

add_executable(unit_tests tests/main.cpp tests/array/unit.cpp tests/bit_string/unit.cpp)
target_link_libraries(unit_tests bit_string array gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_tests)

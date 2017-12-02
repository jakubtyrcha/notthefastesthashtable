cmake_minimum_required(VERSION 3.8)
project(hashtable)

set(CMAKE_CXX_STANDARD 17)

include_directories(/Users/jakubtyrcha/Code/catch2/include)
include_directories(/Users/jakubtyrcha/Code/nonius/include)

set(SOURCE_FILES hashtable.h)
add_executable(hashtable ${SOURCE_FILES} main.cpp utilities.cpp)

add_executable(tests ${SOURCE_FILES} hashtable_test.cpp)
add_executable(benchmarks ${SOURCE_FILES} hashtable_benchmark.cpp)

set(CMAKE_CXX_FLAGS "-O2")
# set(CMAKE_CXX_FLAGS "-fsanitize=address")
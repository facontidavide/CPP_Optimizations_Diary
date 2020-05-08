cmake_minimum_required(VERSION 2.8)

project(just_a_string)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# On ubuntu, install with: sudo apt install libbenchmark-dev
# https://github.com/google/benchmark
find_package(benchmark REQUIRED)
find_package(Threads)

add_executable(just_a_string  just_a_string.cpp)
target_link_libraries(just_a_string benchmark Threads::Threads)

add_executable(small_strings  small_strings/small_strings.cpp)
target_link_libraries(small_strings benchmark Threads::Threads)

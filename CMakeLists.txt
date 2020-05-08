cmake_minimum_required(VERSION 2.8)

project(cpp_optimizations)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# On ubuntu, install with: sudo apt install libbenchmark-dev
# https://github.com/google/benchmark
find_package(benchmark REQUIRED)
find_package(Threads)

function(compile_example  target file)
    add_executable(${target}  ${file} )
    target_link_libraries(${target}  benchmark Threads::Threads)
endfunction()

compile_example(prefer_references          basics/prefer_references/prefer_references.cpp)

compile_example(vector_reserve             vectors_everywhere/reserve/reserve.cpp)

compile_example(strings_are_vectors        just_a_string/strings_are_vectors/strings_are_vectors.cpp)
compile_example(small_string_optimization  just_a_string/small_strings/small_strings.cpp)


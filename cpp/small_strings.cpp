#include <benchmark/benchmark.h>
#include <cstring>

static const char* SHORT_STR = "hello world";

static void ShortStringCreation(benchmark::State& state) {
  // Create a string over and over again.
  // It is just because "short strings optimization" is active
  // no memory allocations
  for (auto _ : state) {
    std::string created_string(SHORT_STR);
    benchmark::DoNotOptimize(created_string);
  }
}
BENCHMARK(ShortStringCreation);

static void ShortStringCopy(benchmark::State& state) {

  // Here we create the string only once, but copy repeatably.
  // Why it is much slower than ShortStringCreation?
  // The compiler, apparently, outsmarted me

  std::string x; // create once
  for (auto _ : state) {
    x = SHORT_STR; // copy
  }
}
BENCHMARK(ShortStringCopy);

static const char* LONG_STR = "this will not fit into small string optimization";

static void LongStringCreation(benchmark::State& state) {
  // The long string will trigget memory allocation for sure

  for (auto _ : state) {
    std::string created_string(LONG_STR);
    benchmark::DoNotOptimize(created_string);
  }
}
BENCHMARK(LongStringCreation);

static void LongStringCopy(benchmark::State& state) {
  // Now we do see an actual advantage, reciclying the same string
  // multiple times
  std::string x;
  for (auto _ : state) {
    x = LONG_STR;
  }
}
BENCHMARK(LongStringCopy);


BENCHMARK_MAIN();

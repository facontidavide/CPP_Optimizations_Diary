#include <benchmark/benchmark.h>
#include <vector>


static void NoReserve(benchmark::State& state) {

  for (auto _ : state) {
    std::vector<size_t> v;
    for(size_t i=0; i<100; i++){
        v.push_back(i);
    }
    benchmark::DoNotOptimize( v );
  }
}
BENCHMARK(NoReserve);

static void WithReserve(benchmark::State& state) {

  for (auto _ : state) {
    std::vector<size_t> v;
    v.reserve(100);
    for(size_t i=0; i<100; i++){
        v.push_back(i);
    }
    benchmark::DoNotOptimize( v );
  }
}
BENCHMARK(WithReserve);


static void ObsessiveRecycling(benchmark::State& state) {

  std::vector<size_t> v;
  for (auto _ : state) {
    v.clear();
    for(size_t i=0; i<100; i++){
        v.push_back(i);
    }
    benchmark::DoNotOptimize( v );
  }
}
BENCHMARK(ObsessiveRecycling);

BENCHMARK_MAIN();

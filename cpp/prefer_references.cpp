#include <benchmark/benchmark.h>
#include <cstring>

struct Vector3D{
    double x;
    double y;
    double z;
};

Vector3D MultiplyByTwo_Value(Vector3D p){
    return { p.x*2, p.y*2, p.z*2 };
}

Vector3D MultiplyByTwo_Ref(const Vector3D& p){
    return { p.x*2, p.y*2, p.z*2 };
}

void MultiplyVector_Value(benchmark::State& state) {
  Vector3D in = {1,2,3};
  for (auto _ : state) {
    Vector3D out = MultiplyByTwo_Value(in);
    benchmark::DoNotOptimize(out);
  }
}
BENCHMARK(MultiplyVector_Value);

void MultiplyVector_Ref(benchmark::State& state) {
  Vector3D in = {1,2,3};
  for (auto _ : state) {
    Vector3D out = MultiplyByTwo_Ref(in);
    benchmark::DoNotOptimize(out);
  }
}
BENCHMARK(MultiplyVector_Ref);
//----------------------------------
size_t GetSpaces_Value(std::string str)
{
    size_t spaces = 0;
    for(const char c: str){
        if( c == ' ') spaces++;
    }
    return spaces;
}

size_t GetSpaces_Ref(const std::string& str)
{
    size_t spaces = 0;
    for(const char c: str){
        if( c == ' ') spaces++;
    }
    return spaces;
}

const std::string LONG_STR("a long string that can't use Small String Optimization");

void PassStringByValue(benchmark::State& state) {
  for (auto _ : state) {
    size_t n = GetSpaces_Value(LONG_STR);
    benchmark::DoNotOptimize(n);
  }
}
BENCHMARK(PassStringByValue);

void PassStringByRef(benchmark::State& state) {
  for (auto _ : state) {
      size_t n = GetSpaces_Ref(LONG_STR);
      benchmark::DoNotOptimize(n);
  }
}
BENCHMARK(PassStringByRef);

//----------------------------------

size_t Sum_Value(std::vector<unsigned> vect)
{
    size_t sum = 0;
    for(unsigned val: vect) { sum += val; }
    return sum;
}

size_t Sum_Ref(const std::vector<unsigned>& vect)
{
    size_t sum = 0;
    for(unsigned val: vect) {  sum += val; }
    return sum;
}

const std::vector<unsigned> vect_in = { 1, 2, 3, 4, 5 };

void PassVectorByValue(benchmark::State& state) {
  for (auto _ : state) {
    size_t n = Sum_Value(vect_in);
    benchmark::DoNotOptimize(n);
  }
}
BENCHMARK(PassVectorByValue);

void PassVectorByRef(benchmark::State& state) {
  for (auto _ : state) {
      size_t n = Sum_Ref(vect_in);
      benchmark::DoNotOptimize(n);
  }
}
BENCHMARK(PassVectorByRef);

//----------------------------------

BENCHMARK_MAIN();

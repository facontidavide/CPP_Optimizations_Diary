#include <benchmark/benchmark.h>
#include <cstring>

enum Color{
    BLUE,
    RED,
    YELLOW
};

std::string ToStringBad(Color c)
{
    switch(c)
    {
    case BLUE:   return "BLUE";
    case RED:    return "RED";
    case YELLOW: return "YELLOW";
    }
}

const std::string& ToStringBetter(Color c)
{
    static const std::string color_name[3] ={"BLUE", "RED", "YELLOW"};
    switch(c)
    {
    case BLUE:   return color_name[0];
    case RED:    return color_name[1];
    case YELLOW: return color_name[2];
    }
}

static void ToStringByValue(benchmark::State& state) {


  for (auto _ : state) {
    std::string x = ToStringBad(BLUE);
    benchmark::DoNotOptimize( x );
  }
}
BENCHMARK(ToStringByValue);

static void ToStringByReference(benchmark::State& state) {

  std::string x; // create once
  for (auto _ : state) {
    const std::string& x = ToStringBetter(BLUE);
    benchmark::DoNotOptimize( x );
  }
}
BENCHMARK(ToStringByReference);

//---------------------------------------------


// Create a new string every time (even if return value optimization may help)
static std::string ModifyString(const std::string& input)
{
    std::string output = input;
    output.append("... indeed");
    return output;
}
// Reuse an existing string that MAYBE, have the space already reserved
// (or maybe not..)
static void ModifyStringBetter(const std::string& input, std::string& output)
{
    output = input;
    output.append("... indeed");
}

static const char* LONG_STR = "this will not fit into small string optimization";


static void ModifyByValuee(benchmark::State& state) {

  std::string in(LONG_STR);
  std::string out;
  // Memory must be allocated every time
  for (auto _ : state) {
    out = ModifyString(in);
  }
}
BENCHMARK(ModifyByValuee);

static void ModifyByReference(benchmark::State& state) {

  std::string in(LONG_STR);
  std::string out;
  // ModifyStringBetter could be as fast as ModifyString, but
  // occasionally faster, because out has the mory already allocated
  // from previous calls
  for (auto _ : state) {
    ModifyStringBetter(in, out);
  }
}
BENCHMARK(ModifyByReference);


BENCHMARK_MAIN();

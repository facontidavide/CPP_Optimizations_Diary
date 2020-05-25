#include <stdio.h>
#include <string.h>

std::string first_str("This is my first string.");
std::string second_str("This is the second string I want to append.");
std::string third_str("This is the third and last string to append."); 

inline size_t StrSize(const char* str)  {
  return strlen(str);
}

inline size_t StrSize(const std::string& str)  {
  return str.size();
}

template <class Head, class... Tail>
size_t StrSize(const Head& head, Tail const&... tail){
  return StrSize(head) + StrSize(tail...);
}

template <class Head>
void StrAppend(std::string& out, const Head& head){
  out = head;
}

template <class Head, class... Args>
void StrAppend(std::string& out, const Head& head, Args const&... args){
  out += head;
  StrAppend(out, args...);
}

template <class... Args> inline
std::string StrCat(Args const&... args){
  size_t tot_size = StrSize(args...);
  std::string out;
  out.reserve(tot_size);

  StrAppend(out, args...);
  return out;
}

static void DefaultConcatenation(benchmark::State& state) {

  for (auto _ : state) {
    std::string big_one = first_str + " " +  second_str + " " +  third_str;
    benchmark::DoNotOptimize(big_one);
  }
}

static void ManualConcatenation(benchmark::State& state) {

  for (auto _ : state) {
    std::string big_one;
    big_one.reserve(first_str.size() + 
                    second_str.size() + 
                    third_str.size() + 
                    strlen(" ")*2 );

    big_one += first_str;
    big_one += " ";
    big_one += second_str;
    big_one += " ";
    big_one += third_str;
    benchmark::DoNotOptimize(big_one);
  }
}

static void VariadicConcatenation(benchmark::State& state) {

  for (auto _ : state) {
    std::string big_one = StrCat(first_str, " ", second_str, " ", third_str);
    benchmark::DoNotOptimize(big_one);
  }
}

// Register the function as a benchmark
BENCHMARK(DefaultConcatenation);
BENCHMARK(ManualConcatenation);
BENCHMARK(VariadicConcatenation);

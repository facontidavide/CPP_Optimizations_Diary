# Small String Optimizations

Remember when I said that "strings are `std::vector<char>` in disguise"?

In practice, very smart folks realized that you may store
small strings inside the already allocated memory.

Given that the size of a `std::string` is **24 bytes** on a 64-bits
platform (to store data pointer, size and capacity), some
very cool tricks allow us to store **statically** up to 23 bytes
before you need to allocate memory.

That has a huge impact in terms of performance!

![](img/relax_sso.jpg)

For the curious minds, here there are some details about the implementation:

- [SSO-23](https://github.com/elliotgoodrich/SSO-23)
- [CppCon 2016: “The strange details of std::string at Facebook"](https://www.youtube.com/watch?v=kPR8h4-qZdk)

According to your version of the compiler, you may have less than 23 bytes, that is
the theoretical limit.

## Example

```C++
const char* SHORT_STR = "hello world";

void ShortStringCreation(benchmark::State& state) {
  // Create a string over and over again.
  // It is just because "short strings optimization" is active
  // no memory allocations
  for (auto _ : state) {
    std::string created_string(SHORT_STR);
  }
}

void ShortStringCopy(benchmark::State& state) {
  // Here we create the string only once, but copy repeatably.
  // Why is it much slower than ShortStringCreation?
  // The compiler, apparently, outsmarted me
  std::string x; // create once
  for (auto _ : state) {
    x = SHORT_STR; // copy
  }
}

const char* LONG_STR = "this will not fit into small string optimization";

void LongStringCreation(benchmark::State& state) {
  // The long string will trigger memory allocation for sure
  for (auto _ : state) {
    std::string created_string(LONG_STR);
  }
}

void LongStringCopy(benchmark::State& state) {
  // Now we do see an actual speed-up, when recycling
  // the same string multiple times
  std::string x;
  for (auto _ : state) {
    x = LONG_STR;
  }
}
```

As you may notice, my attempt to be clever and say "I will not create a new string
every time" fails miserably if the string is short, but has a huge impact if the string
is allocating memory.

![](img/sso_in_action.png)




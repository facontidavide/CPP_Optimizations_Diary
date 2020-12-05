# Preface: about me

 My name is [Davide Faconti](https://twitter.com/facontidavide) and my job is one of the best in the world: I work in **robotics**.
  
This blog/repository is maintained in **my spare time** and it is not  related to my work there. Therefore *opinions (and memes) are all mine and don't represent my employer in any way*.

I love C++ programming and Open Source and this "diary" is my small contribution to the OSS community.

# CPP Optimizations Diary

Optimizing code in C++ is something that no one can resist. You can have fun
and pretend that you are doing something useful for your organization at the same time!

In this repository, I will record some simple design patterns to improve your code 
and remove unnecessary overhead in **C++**.

If you are a seasoned C++ expert, you probably have your own set of rules already.

These rules help you look like a bad-ass/rockstar/10X engineer to your colleagues.

You are the kind of person that casually drops a [std::vector<>::reserve](docs/reserve.md) before a loop and
nods, smiling, looking at the performance improvement and the astonishment of your team member.

<p align="center"><img src="docs/img/boom.gif" width="350"></p>


Hopefully, the examples in this repository will help you achieve this status of guru
and, as a side effect, save the planet from global warming, sparing useless CPU
cycles from being wasted.

Then, unfortunately, someone on the other side of the planet will start mining Bitcoins or write her/his 
application in **Python** and all your effort to save electricity was for nothing.

I am kidding, Python developers, we love you!

> Narrator: "he was not kidding..."

## Rule 1: measure first (using _good_ tools)

The very first thing any person concerned about performance should do is:
 
- **Measure first** and **make hypothesis later**.

Me and my colleagues are almost always wrong about the reasons a piece of code is slow. 

Sometimes we are right, but it is really hard to know in advance how refactoring will
improve performance. Good profiling tools show in minutes the "low hanging fruits": minimum work, maximum benefit!

Summarizing: 10 minutes profiling can save you hours of guessing and refactoring.

My "goto" tools in Linux are [Hotspot](https://github.com/KDAB/hotspot) and 
[Heaptrack](https://github.com/KDE/heaptrack). I understand Windows has similar
tools too.

<p align="center"><img src="docs/img/hotspot_heaptrack.jpg" width="350"></p>

In the benchmark war, if you are the soldier, these are your rifle and hand grenades.

Once you know which part of the code deserves to be optimized, you might want to use
[Google Benchmark](https://github.com/google/benchmark) to measure the time spent in a very specific
class or function.

You can even run it Google Benchmark online here: [quick-bench.com](http://quick-bench.com/G7B2w0xPUWgOVvuzI7unES6cU4w).

![quick-bench](docs/img/quick-bench.png)

## Rule 2: learn good design patterns, use them by default

Writing good code is like brushing your teeth: you should do it without thinking too much about it.

It is a muscle that you need to train, that will become stronger over time. But don't worry:
once you start, you will start seeing recurring patterns that 
are surprisingly simple and works in many different use cases.

**Spoiler alert**: one of my most beloved tricks is to _minimize the number of heap allocations_.
You have no idea how much that helps.

But let's make something absolutely clear: 

- Your **first goal** as a developer (software engineer?) is to create code that is **correct** and fulfil the requirements.
- The **second** most important thing is to make your code **maintainable and readable** for other people.
- In many cases, you also want to make code faster, because [faster code is better code](https://craigmod.com/essays/fast_software/).

In other words, think twice before doing any change in your code that makes it less readable or harder to debug,
just because you believe it may run 2.5% faster.

# Get started

For a more comfortable reading experience, visit: https://cpp-optimizations.netlify.app

## Optimization examples

### "If you pass that by value one more time..."

- [Use Const reference by default](docs/prefer_references.md).

- Move semantic (TODO).

- Return value optimization (TODO).


### std::vector<> is your best friend


- [Use std::vector<>::reserve by default](docs/reserve.md)

- ["I have learnt linked-lists at university, should I use them?" Nooope](docs/no_lists.md).

- [You don't need a `std::map<>` for that](docs/dont_need_map.md).

- [Small vector optimization](docs/small_vectors.md)


### "It is just a string, how bad could that be?"

- [Strings are (almost) vectors](docs/strings_are_vectors.md)

- [When not to worry: small string optimization](docs/small_strings.md).

- [String concatenation: the false sense of security of `operator+`](docs/strings_concatenation.md).

- `std::string_view`: love at first sight (TODO).

### Don't compute things twice.

- [Example: 2D/3D transforms the right way](docs/2d_transforms.md).

- [Iterating over a 2D matrix: less elegant, more performant](docs/2d_matrix_iteration.md).

### Fantastic data structures and where to find them.

- [I tried `boost::container::flat_map`. You won't imagine what happened next](docs/boost_flatmap.md).

### Case studies

- [Simpler and faster way to filter Point Clouds in PCL.](docs/pcl_filter.md)

- [Fast Palindrome: the cost of conditional branches](docs/palindrome.md)


# License

<p xmlns:dct="http://purl.org/dc/terms/" xmlns:cc="http://creativecommons.org/ns#" class="license-text">This work   is licensed under <a rel="license" href="https://creativecommons.org/licenses/by-sa/4.0">CC BY-SA 4.0<img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/cc.svg?ref=chooser-v1" /><img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/by.svg?ref=chooser-v1" /><img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/sa.svg?ref=chooser-v1" /></a></p>

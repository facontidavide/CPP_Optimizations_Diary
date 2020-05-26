# String concatenation

Warning: before you read this, remember the rule #1 we mentioned at the beginning.

**Optimize your code only if you can observe a visible overhead 
with you profiling tools**. Said that...

As we said, strings are a little more than vectors of characters, therefore
they may need heap allocations to store all their elements.

Concatenating strings in C++ is very easy, but there is something we should
be aware of.

## "Default" concatenation

Look at this familiar line of code. 

```C++
std:string big_string = first + " " + second + " " + third;

// Where...
// std::string first("This is my first string.");
// std::string second("This is the second string I want to append.");
// std::string third("This is the third and last string to append."); 
```

Noticing anything suspicious? Think about heap allocations...

![](img/spider_senses.png)

Let me rewrite it like this:

```C++
std:string big_string = (((first + " ") + second) + " ") + third;
```

Hopefully you got it. To concatenate strings of this length, you will 
need multiple heap allocations and copies from the old memory block to the new one.

If only `std::string` had a method similar to `std::vector::reserve()` :(

Hey, wait... [what is this](https://en.cppreference.com/w/cpp/string/basic_string/reserve)?


## "Manual" concatenation

Let's use reserve to reduce the amount of heap allocations to exactly one.

We can calculate the total amount of characters needed by `big_string` in
advance and reserve it like this:

```C++
    std::string big_one;
    big_one.reserve(first_str.size() + 
                    second_str.size() + 
                    third_str.size() + 
                    strlen(" ")*2 );

    big_one += first;
    big_one += " ";
    big_one += second;
    big_one += " ";
    big_one += third;
```

I know what you are thinking and you are 100% right.

![](img/feel_bad.jpg)

That is a horrible piece of code... that is **2.5 times faster** than the 
default string concatenation!

## Variadic concatenation

Can we create a string concatenation function that is fast, reusable **and** 
readable?

We do, but we need to use some heavy weapons of Modern C++: **variadic templates**.

There is a very [nice article about variadic templates here](https://arne-mertz.de/2016/11/more-variadic-templates/),
that you should probably read if you are not familiar with them.


```C++
//--- functions to calculate the total size ---
size_t StrSize(const char* str) {
  return strlen(str);
}

size_t StrSize(const std::string& str) {
  return str.size();
}

template <class Head, class... Tail>
size_t StrSize(const Head& head, Tail const&... tail) {
  return StrSize(head) + StrSize(tail...);
}

//--- functions to append strings together ---
template <class Head>
void StrAppend(std::string& out, const Head& head) {
  out = head;
}

template <class Head, class... Args>
void StrAppend(std::string& out, const Head& head, Args const&... args) {
  out += head;
  StrAppend(out, args...);
}

//--- Finally, the function to concatenate strings ---
template <class... Args> 
std::string StrCat(Args const&... args) {
  size_t tot_size = StrSize(args...);
  std::string out;
  out.reserve(tot_size);

  StrAppend(out, args...);
  return out;
}
```

That was a lot of complex code, even for a trained eye. But the good news are
that it is very easy to use:

```C++
std:string big_string = StrCat(first, " ", second, " ", third );
```

So, how fast is that?

![](img/string_concatenation.png)

The reason why the version with variadic templates is slightly slower than
the "ugly" manual concatenation is... 

I have no idea!

What I **do** know is that it is twice as fast as the default one and
it is not an unreadable mess. 

## Before you copy and paste my code...

My implementation of `StrCat` is very limited and I just wanted to make a point:
beware of string concatenations in C++.

Nevertheless, don't think it twice and use [{fmt}](https://github.com/fmtlib/fmt) instead.

Not only it is an easy to integrate, well documented and **very**
fast library to format strings.

It is also an implementation of [C++20 std::format](https://en.cppreference.com/w/cpp/utility/format).

This means that you can write code that is readable, performant and future proof! 







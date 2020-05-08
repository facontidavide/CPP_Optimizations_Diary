# It is just a string: should I worry?

 `std::string` is a wonderful abstraction, when compared to the awful mess of
 raw pointers and lengths that you have to deal with in **C**.

I am kidding, C developers, we love you!
> Or we sympathize for you, depends on how you want to look at it.

If you think about it, it should be no more than an `std::vector<char>` in disguise,
with some useful utility that makes sense for text, but no much more.

On one hand, **it is**, but here comes what is called **Small String Optimization (SSO)**.

[Read more about SSO here](../small_strings/).

What I want to show you here is that, as any objects that **might**
require memory allocation, you must use best practices you should use
with similar containers (even if, arguably, often you need to worry less).

## ToString

```c++
enum Color{
    BLUE,
    RED,
    YELLOW
};

std::string ToStringBad(Color c)
{
    switch(c) {
    case BLUE:   return "BLUE";
    case RED:    return "RED";
    case YELLOW: return "YELLOW";
    }
}

const std::string& ToStringBetter(Color c)
{
    static const std::string color_name[3] ={"BLUE", "RED", "YELLOW"};
    switch(c) {
    case BLUE:   return color_name[0];
    case RED:    return color_name[1];
    case YELLOW: return color_name[2];
    }
}
```

This is just an example of how, if you can, you should not create over and
over a string. Of course, I can hear you arguing:

"Davide, you are forgetting Return Value Optimization"?

I am not. But a `const&` is **always** guaranteed to be the most
performing option, so why try your luck?

<p align="center"><img src="tostring.png"></p>

## Reuse temporary strings

Here it comes a similar example in which we **potentially**
recycle the memory already allocated in the past.

You are not guaranteed to be faster with the latter version, but you
might be.

```c++
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
```

And, as expected...

<p align="center"><img src="modifystring.png"></p>



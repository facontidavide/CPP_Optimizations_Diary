# Case study: filter a Point Cloud faster

[Point Cloud Library (PCL)](https://pointclouds.org/) is a very popular
library used in robotics, autonomous vehicles and 3D perception in general. 
Its contribution to this fields is **huge**.

![](img/pcl.jpg)

It is a behemoth with more than [12K commits and 5K Github stars](https://github.com/PointCloudLibrary/pcl).

With about [400 contributors](https://github.com/PointCloudLibrary/pcl/graphs/contributors)
you would think that it is hard to find opportunities for optimization
in plain sight, even more inside the most frequently used functions.

**Me**: Muahahahaha (Evil laugh)

## The ConditionalRemoval filter

I bet that anyone that has been playing around with PCL has used 
[pcl::ConditionalRemoval](https://pcl-tutorials.readthedocs.io/en/latest/remove_outliers.html) at least once.

From the [official tutorial](https://pcl-tutorials.readthedocs.io/en/latest/remove_outliers.html):

```C++
// slightly changed for clarity
using namespace pcl;
auto range_cond  = std::make_shared<ConditionAnd<PointXYZ> ();
range_cond->addComparison ( 
    std::make_shared<FieldComparison<PointXYZ>("z", ComparisonOps::GT, 0.0));
range_cond->addComparison (
    std::make_shared<FieldComparison<PointXYZ>("z", ComparisonOps::LT, 1.0)));

// build the filter
ConditionalRemoval<PointXYZ> condition_removal;
condition_removal.setCondition (range_cond);
condition_removal.setInputCloud (input_cloud);
// apply filter
condition_removal.filter (*cloud_filtered);
```

> Basically, we create the condition which a given point must satisfy for it to remain in our PointCloud. 
> In this example, we use add two comparisons to the condition: greater than (GT) 0.0 and less than (LT) 1.0. 
> This condition is then used to build the filter.

Let me rephrase it for people that are not familiar with PCL:

- An object is created that says: "the Z value of the point must be greater than 0.0".
- Another object is created saying: "the Z value of the point must be less than 1.0".
- They are both added to a `ConditionAnd`.
- We tell `ConditionalRemoval` to use this combined condition.
- Apply the filter to an input cloud to create a filtered one.

Now think that the usal Point Cloud has a number of points in the order of 
**tens of thousands** or more.
 
 Think about it:
 
![](img/think_about_it.jpg)

Seriously, take some minutes to think how, given a **vector** of points that looks like this:

```C++
// oversimplified, not the actual implementation
struct PointXYZ{
  float x;
  float y;
  float z; 
};
```

You want to create another point cloud with all the points that pass this condition:

      0.0 < point.z < 1.0
      
I mean, if you ask **me**, this is what I would do, because I am not that smart:

```C++
auto cloud_filtered = std::make_shared<PointCloud<PointXYZ>();

for (const auto& point: input_cloud->points) 
{
  if( point.z > 0.0 && point.z < 1.0 )
  {
    cloud_filtered->push_back( point );
  }
}
``` 
This is what we will call the **"naive filter"**.

Before showing you a benchmark that will knock your socks off (don't worry, I will),
I must admit that it is an **unfair comparison** because the `pcl` filters
do many more checks when processing your data to prevent weird corner cases.


But do not forget that we expressed our conditions like this:

```C++
pcl::FieldComparison<pcl::PointXYZ> ("z", pcl::ComparisonOps::GT, 0.0)));
pcl::FieldComparison<pcl::PointXYZ> ("z", pcl::ComparisonOps::LT, 1.0)));
```

If you think about it, there **must be** some kind of parser "somewhere".

The easiest implementation of a parser is of course a `switch`
statement, but no one would ever do that for **each** of these trillion points...

[Oh, snap!](https://github.com/PointCloudLibrary/pcl/blob/pcl-1.11.0/filters/include/pcl/filters/impl/conditional_removal.hpp#L98-L127)

Indeed, 2 switch statements called for each point of the cloud.

Summarizing: the very fact that these function tries to be "too clever"
using these "composable rules", means that the implementation is **inherently slow**.

There is nothing we can do to save them. Nevertheles, we can replace them ;)

## Davide, give me speed AND expressive code.

Sure thing, my friend!

Since `pcl::FieldComparison` is intrinsically broken (so are all the other Conditions in the library), 
because of their `switch`statements, let me write my own pcl::Condition (must be derived from `pcl::ConditionBase`) like this:

```C++
template <typename PointT>
class GenericCondition : public pcl::ConditionBase<PointT>
{
public:
  typedef std::shared_ptr<GenericCondition<PointT>> Ptr;
  typedef std::shared_ptr<const GenericCondition<PointT>> ConstPtr;
  typedef std::function<bool(const PointT&)> FunctorT;

  GenericCondition(FunctorT evaluator): 
    pcl::ConditionBase<PointT>(),_evaluator( evaluator ) 
  {}

  virtual bool evaluate (const PointT &point) const {
    // just delegate ALL the work to the injected std::function
    return _evaluator(point);
  }
private:
  FunctorT _evaluator;
};
```

That is literally **all** the code you need, no omissions.

I am simply wrapping a `std::function<bool(const PointT&)>` inside  `pcl::ConditionBase`. Nothing else.
 
This is the **old** code:


```C++
auto range_cond  = std::make_shared<ConditionAnd<PointXYZ> ();
range_cond->addComparison ( 
    std::make_shared<FieldComparison<PointXYZ>("z", ComparisonOps::GT, 0.0));
range_cond->addComparison (
    std::make_shared<FieldComparison<PointXYZ>("z", ComparisonOps::LT, 1.0)));
```

And this is the **new** one, where my condition is expressed in plain old code:

```C++   
auto range_cond = std::make_shared<GenericCondition<PointXYZ>>(
  [](const PointXYZ& point){ 
      return point.z > 0.0 && point.z < 1.0; 
  });
```

The rest of the code is unchanged!!!

![](img/beautiful.jpg)

## Let's talk about speed

You may find the code to replicate my tests [here](https://github.com/facontidavide/CPP_Optimizations_Diary/tree/master/cpp/pcl_conditional_removal.cpp).

These are the benchmarks based on my sample cloud and 4 filters (upper and lower bound in X and Y):

```
-------------------------------------------------------------
Benchmark                   Time             CPU   Iterations
-------------------------------------------------------------
PCL_Filter            1403083 ns      1403084 ns          498
Naive_Filter           107418 ns       107417 ns         6586
PCL_Filter_Generic     668223 ns       668191 ns         1069
```
Your results may change a lot according to the number of conditions and the size of the point cloud.

But the lessons to learn are:

- The "naive" filter might be an option in many cases and it is blazing fast.
- The "safe" `pcl::ConditionalRemoval` can still be used if you just ditch the builtin `pcl::Conditions` and use instead the much more concise and readable `GenericCondition`.


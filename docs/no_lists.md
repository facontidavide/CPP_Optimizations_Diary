# If you are using std::list<>, you are doing it wrong

<p align="center"><img src="../images/linked_list.png" width="400"></p>

I am not wasting time here to repeat benchmarks which a lot of people did already.

- [std::vector vs std::list benchmark](https://baptiste-wicht.com/posts/2012/11/cpp-benchmark-vector-vs-list.html)

- [Are lists evil? Bjarne Stroustrup](https://isocpp.org/blog/2014/06/stroustrup-lists)

- [Video from Bjarne Stroustrup keynote](https://www.youtube.com/watch?v=YQs6IC-vgmo)

You think your case is special, a unique snowflake. **It is not**. 

You have another STL data structure that is better than `std::list`:
 [std::deque<>](https://es.cppreference.com/w/cpp/container/deque) almost 99% of the time. 
 
 In some cases, even the humble `std::vector` is better than a list.
 
If you like very exotic alternatives have a look at [plf::colony](https://plflib.org/colony.htm).
 
But seriously, just use `vector`or `deque`.

## Real world example: improving the Intel RealSense driver

This is a practical example of a Pull Request I sent to the [RealSense](https://github.com/IntelRealSense)
repository a while ago.

<p align="center"><img src="../images/realsense.png" width="450"></p>

They where using that abomination called `std::list<>` for a reason that I can not understand.

Just kidding, Intel Developers, we love you!

Here you can find the link to the Pull Request:
- [Considerable CPU saving in BaseRealSenseNode::publishPointCloud()](https://github.com/IntelRealSense/realsense-ros/pull/1097)

In a nutshell, the whole PR contains only two tiny changes:

```C++
// We changed this list, created at each camera frame
std::list<unsigned> valid_indices;

// With this vector: a class member, that is cleared before reusing
// (but allocated memory is still there)
std::vector<unsigned> _valid_indices;
```

Additionally, we have a quite large object called `sensor_msgs::PointCloud2 msg_pointcloud` that
is converted into a class member that is reused over and over again at each frame.

The reported speed improvement is 20%-30%, that is huge, if you think about it.



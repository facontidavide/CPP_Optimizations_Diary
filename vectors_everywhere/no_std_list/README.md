# If you are using std::list<>, you are doing it wrong

<p align="center"><img src="linked_list.png" width="350"></p>

I am not wasting time here to repeat benchmarks which a lot of people did already.

- [std::vector vs std::list benchmark](https://baptiste-wicht.com/posts/2012/11/cpp-benchmark-vector-vs-list.html)

- [Are lists evil? Bjarne Stroustrup](https://isocpp.org/blog/2014/06/stroustrup-lists)

- [Video from Bjarne Stroustrup keynote](https://www.youtube.com/watch?v=YQs6IC-vgmo)

You think your case is special, a unique snowflake. **It is not**. 

You have another STL data structure that is almost 99% of the times better than `std::list`:
 [std::deque<>](https://es.cppreference.com/w/cpp/container/deque). 
 
 In some cases, even the humble `std::vector` is better than a list.
 
If you like very exotic alternatives have a look at [plf::colony](https://plflib.org/colony.htm).
 
But seriously, just use `vector`or `deque`.





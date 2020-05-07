# If you are using std::list<>, you are doing it wrong

![linked lists](linked_list.jpg)

I am not wasting time here to repeat benchmark that a lot of people did already.

- [std::vector vs std::list benchmark](https://baptiste-wicht.com/posts/2012/11/cpp-benchmark-vector-vs-list.html)

- [Are lists evil? Bjarne Stroustrup](https://isocpp.org/blog/2014/06/stroustrup-lists)

- [Video from Bjarne Stroustrup keynote](https://www.youtube.com/watch?v=YQs6IC-vgmo)

You think you case is special. It is not. Just try using `std::vector<>`
 (with `reserve` if you can) and you will see.
 
 If you really believe that your case is different, have a look to
 [plf::colony](https://plflib.org/colony.htm).
 
  




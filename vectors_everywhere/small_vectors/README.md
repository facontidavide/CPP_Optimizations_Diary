# Small vector optimization 

By now, I hope I convinced you that `std::vector` is the first data structure that you should consider to use unless you need an associative container.

But even if we cleverly use `reserve` to prevent superfluous heap allocations and copies when we push elements into the vector, there will be a least **one** heap allocation at the beginning. Can we do better? 

Sure we can! if you have read already about the [small string optimization](../../just_a_string/small_strings) you know where this is going.

# "Static vectors" and "Small vectors"

When you are sure that your vector is small and will remain small-ish even in the worst-case scenario, you can allocate the entire array of elements in the stack, without any heap allocation.

In other words, you want the familiar API of this:
```C++
std::vector<double> my_data; // at least one heap allocation
```
When in fact, under the hood, you have something like this
```C++
double my_data[MAX_SIZE]; // no heap allocations 
int size_my_data;
```
You may think that this is unlikely, but you will be surprised to know that this happens much more often than you may expect. Just 2 weeks ago, I identified this very same pattern in one of our libraries, where the size of some vector could be any number between 0 and 8 at most.

 Let's see how a simple and naive implementation of `StaticVector` might look like:
```C++
#include <array>
#include <initializer_list>

template <typename T, size_t N>
class StaticVector
{
public:

  using iterator       = typename std::array<T,N>::iterator;
  using const_iterator = typename std::array<T,N>::const_iterator;

  StaticVector(uint8_t n=0): _size(n) {
    if( _size > N ){
      throw std::runtime_error("SmallVector overflow");
    }
  }

  StaticVector(const StaticVector& other) = default;
  StaticVector(StaticVector&& other) = default;

  StaticVector(std::initializer_list<T> init)
  {
    _size = init.size();
    for(int i=0; i<_size; i++) { _storage[i] = init[i]; }
  }

  void push_back(T val){
    _storage[_size++] = val;
    if( _size > N ){
      throw std::runtime_error("SmallVector overflow");
    }
  }

  void pop_back(){
    if( _size == 0 ){
      throw std::runtime_error("SmallVector underflow");
    }
    back().~T(); // call destructor
    _size--;
  }

  size_t size() const { return _size; }

  void clear(){ while(_size>0) { pop_back(); } }

  T& front() { return _storage.front(); }
  const T& front() const { return _storage.front(); }

  T& back() { return _storage[_size-1]; }
  const T& back() const { return _storage[_size-1]; }

  iterator begin() { return _storage.begin(); }
  const_iterator begin() const { return _storage.begin(); }

  iterator end() { return _storage.end(); }
  const_iterator end() const { return _storage.end(); }

  T& operator[](uint8_t index) { return _storage[index]; }
  const T& operator[](uint8_t index) const { return _storage[index]; }

  T& data() { return _storage.data(); }
  const T& data() const { return _storage.data(); }

private:
  std::array<T,N> _storage;
  uint8_t _size = 0;

```

**StaticVector** looks like a `std::vector` but is...

<p align="center"><img src="inconceivably.jpg" width="350"></p>

Ifn other case there is a very high probability that a vector-like container will have at most **N** elements, but we are not "absolutely sure" about that.

We can still use a container, generally known as **SmallVector**, that will use the pre-allocated  memory from the stack for its first N alements and **only** if and when the container needs to grow further, will create a new storage block using heap allocation.

## StaticVector and SmallVector in the wild

It turn out that these tricks are well known and can be found already implemented and ready to use in many popular libraries:

- [Boost::cotainer](https://www.boost.org/doc/libs/1_73_0/doc/html/container.html). If it exist, Boost have it of course.
- [Abseil](https://github.com/abseil/abseil-cpp/tree/master/absl/container). They are called `fixed_array` and `inlined_vector`. 
- For didactic purposem you may have a look to the [LLVM SmallVector](https://github.com/llvm/llvm-project/blob/master/llvm/include/llvm/ADT/SmallVector.h)


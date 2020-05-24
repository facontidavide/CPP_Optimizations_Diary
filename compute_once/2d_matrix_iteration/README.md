# Iterating over a 2D matrix

2D matrices are very common in my domain (robotics).

We use them to represent images, gridmaps/costmaps, etc.

Recently, I realized that one of our algorithms dealing with costmaps was quite slow,
 and I profiled it using **Hotspot**.

I realized that one of the bottlenecks was a function that combined together two costmaps to create
a third one. The function looked qualitatively like this:

```C++
// this is over simplified code, do not argue about it with me
for( size_t y = y_min; y < y_max; y++ ) 
{
    for( size_t x = x_min; x < x_max; x++ ) 
    {
        matrix_out( x,y ) = std::max( mat_a( x,y ), mat_b( x,y ) ); 
    }
}
```

Pretty straightforward, right? Elegantly written, as it should be.
But since my measurements revealed that it was using too much CPU, I decided to 
follow the white rabbit inside the optimization hole.

## How do you write a good 2D matrix in C++?

Have you ever written code like this?

```C++
// My computer science professor did it like this
float** matrix = (float**) malloc( rows_count * sizeof(float*) );
for(int r=0; r<rows_count; r++) 
{
    matrix[r] = (float*) malloc( columns_count * sizeof(float) );
}
// access an element of the matrix like this:
matrix[row][col] = 42;
```

Then you are grounded. The best you can do with this code is to take it to
[Mount Doom in Mordor](https://en.wikipedia.org/wiki/Mount_Doom) and throw it inside the volcano.

<p align="center"><img src="mordor.jpg" width="350"></p>


First of all, use [Eigen](http://eigen.tuxfamily.org). It is a wonderful library, that is performant and has a beautiful API.

Secondary, just **for didactic purposes**, this is the way you should implement an efficient matrix (but don't, use Eigen, seriously). It is relevant to show it here, because this is the approach that everybody follows, including Eigen.

```C++
template <typename T> class Matrix2D
{
public:
    Matrix2D(size_t rows, size_t columns):  _num_rows(rows)
    {
        _data.resize( rows * columns );
    }
    
    size_t rows() const
    { 
    	return _num_rows; 
    }
    
    T& operator()(size_t row, size_t col)  
    {
        size_t index = col*_num_rows + row; 
        return _data[index];
    }
    
    T& operator[](size_t index)  
    {
        return _data[index];
    }
    
    // all the other methods omitted
private:
    std::vector<T> _data;
    size_t _num_rows;
};

// access an element of the matrix like this:
matrix(row, col) = 42;

```

This is the most cache-friendly way to build a matrix, with a single memory allocation and 
the data stored in a way known as [column-wise](https://www.geeksforgeeks.org/row-wise-vs-column-wise-traversal-matrix/).

To convert a row/column pair into an index in the vector, we need a single multiplication and addition.

## Back to my problem

Do you remember the code we wrote at the beginning?

We have a number of iterations that is equal to `(x_max-x_min)*(y_max-y_min)`.  
Often, that it is a lot of pixels/cells.

In each iteration, we calculate the index 3 times using the formula:

     size_t index = col*_num_rows + row;

Holy moly, that is a lot of multiplications!

It turned out that it was worth rewriting the code like this:

```C++
// calculating the index "by hand"
for(size_t y = y_min; y < y_max; y++) 
{
    size_t offset_out =  y * matrix_out.rows();
    size_t offset_a   =  y * mat_a.rows();
    size_t offset_b   =  y * mat_b.rows();
    for(size_t x = x_min; x < x_max; x++) 
    {
        size_t index_out =  offset_out + x;
        size_t index_a   =  offset_a + x;
        size_t index_b   =  offset_b + x;
        matrix_out( index_out ) = std::max( mat_a( index_a ), mat_b( index_b ) ); 
    }
}
```

So, I know what you are thinking, **my eyes are hurting too**. It is ugly. But the performance boost was too much to ignore.

That is not surprising, considering that the number of multiplications is dramatically reduced by a factor `(x_max-x_min)*3`.



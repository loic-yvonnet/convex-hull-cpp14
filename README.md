<h1>Convex Hull Algorithms in C++14</h1>

This small library provides a mono-threaded implementation for Graham Scan, Monotone Chain, Jarvis March and Chan's algorithms for the computation of the convex hull of a set of points in the 2D space.

<h2>Build prerequisites</h2>

The implementation is written in C++14 and requires a fully conformant C++14 compiler. Some features of C++17 are also used, but C++17 is not very different from C++14 so it should be easy to backport to C++14 if required.

The code compiles and runs successfully with clang-900.0.37 on Mac OS X 10.2. It should compile with most recent versions of Clang and GCC.

It also requires CMake 3.9:

```
cmake .
make
test/hull_unit_tests
```

<h2>Usage</h2>

<h3>About points</h3>

You can use your own definition of a point with the algorithms, if it contains (either):
<ul>
<li>public <code>x</code> and <code>y</code> data members (or <code>X</code> and <code>Y</code>, upper case).</li>
<li>public <code>x</code> and <code>y</code> methods (or <code>X</code> and <code>Y</code>, upper case).</li>
<li>If your definition of a point provides different kinds of accessors to the coordinates, you can provide your specialization for the <code>is_point</code> template structure, and the <code>x()</code> and <code>y()</code> template functions.</li>

<h3>Examples</h3>

The following code snippet demonstrates the way to use the iterator-based version of the algorithms:

```cpp
#include "hull/algorithms.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

struct point {
    double x, y;
};

int main() {
    std::vector<point> points{
        {5., 3.}, {8., 12.}, {3.14, 42.}
    };
    std::vector<point> convex_hull(points.size());
    
    const auto last = hull::compute_convex_hull(std::begin(points), std::end(points), std::begin(convex_hull));
    
    std::for_each(std::begin(convex_hull), last, [](const auto& p) {
        std::cout << "(" << p.x << ";" << p.y << ") ";
    });
    
    std::cout << std::endl;
}
```

The following code snippet shows the way to use the container-based version of the algorithms:

```cpp
    std::vector<point> points{
        {5., 3.}, {8., 12.}, {3.14, 42.}
    };
    std::vector<point> convex_hull;
    
    hull::convex::compute(points, convex_hull);
    
    for (const auto& p: convex_hull) {
        std::cout << "(" << p.x << ";" << p.y << ") ";
    };
    
    std::cout << std::endl;
```

<h3>Library documentation</h3>

All the algorithms are defined in header <code>algorithms.hpp</code>, in the namespace <code>hull</code>.

<h4>Iterator-based algorithms</h4>

```cpp
    template <typename RandomIt1, typename RandomIt2>
    RandomIt2 compute_convex_hull(Policy policy, RandomIt1 first, RandomIt1 last, RandomIt2 first2); // (1)
    
    template <typename Policy, typename RandomIt1, typename RandomIt2>
    RandomIt2 compute_convex_hull(RandomIt1 first, RandomIt1 last, RandomIt2 first2); // (2)
    
    template <typename RandomIt1, typename RandomIt2>
    RandomIt2 compute_convex_hull(RandomIt1 first, RandomIt1 last, RandomIt2 first2); // (3)
```

For all these algorithms:
<ul>
<li><code>first</code> is the random access iterator to the first point of the container.</li>
<li><code>last</code> is the random access iterator to the one-past last point of the container.</li>
<li><code>first2</code> is the random access iterator to the first point of the destination container.</li>
<li>it returns the iterator to the last element forming the convex hull of the destination container of points.</li>
<li>The average time complexity is O(N * log(N)).</li>
</ul>

Specificities:

(1) The first parameter is a policy for the choice of the algorithm. It may be either <code>hull::choice::graham_scan</code>, <code>hull::choice::monotone_chain</code>, <code>hull::choice::jarvis_march</code> or <code>hull::choice::chan</code>.

(2) The policy is passed as a template parameter. It may be either <code>hull::graham_scan_t</code>, <code>hull::monotone_chain_t</code>, <code>hull::jarvis_march_t</code> or <code>hull::chan_t</code>.

(3) The default algorithm is Graham Scan.

<h4>Container-based algorithms</h4>

In the namespace <code>hull::convex</code>.

```cpp
    template <typename TContainer1, typename TContainer2>
    void compute(Policy policy, const TContainer1& c1, TContainer2& c2); // (1)
    
    template <typename Policy, typename TContainer1, typename TContainer2>
    void compute(const TContainer1& c1, TContainer2& c2); // (2)
    
    template <typename TContainer1, typename TContainer2>
    void compute(const TContainer1& c1, TContainer2& c2); // (3)
```

For all these algorithms:
<ul>
<li><code>c1</code> is the input container of points.</li>
<li><code>c2</code> is the destination container that will contain the convex hull.</li>
</ul>

Specificities:

(1) The first parameter is a policy for the choice of the algorithm. It may be either <code>hull::choice::graham_scan</code>, <code>hull::choice::monotone_chain</code>, <code>hull::choice::jarvis_march</code> or <code>hull::choice::chan</code>.

(2) The policy is passed as a template parameter. It may be either <code>hull::graham_scan_t</code>, <code>hull::monotone_chain_t</code>, <code>hull::jarvis_march_t</code> or <code>hull::chan_t</code>.

(3) The default algorithm is Graham Scan.

<h2>Required enhancements</h2>

The following enhancements would be the next natural steps for this library:
<ul>
<li>Implement Quickhull.</li>
<li>Implement - or integrate an existing - tiny executor facility (thread pool + task queue).</li>
<li>Implement a parallel version of Quickhull thanks to this executor.</li>
<li>Perform a benchmark between the implementations of Graham Scan, Monotone Chain, Jarvis March and Chan's algorithm.</li>
<li>Perform a benchmark with OpenCV and boost::geometry.</li>
<li>Implement a GPGPU version of Quickhull with OpenCL.</li>
<li>Use Bandit for unit testing instead of a home-made library.</li>
<li>In C++20, everything should be rewritten with concepts, modules, ranges and reflection.</li>
</ul>

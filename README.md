# C++ Extension with Mixin

This simple header-only library add useful functions in STL.

```C++
vector<int> a{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
auto b = a.filter([](auto &value) {
    return value % 2 == 0;
}).map([](auto &value) {
    return value * 10;
}).for_each_indexed([&b](auto idx, auto &value) {
    cout << value << (idx < b.size() - 1 ? ", " : "");
});
```

All things happen at compile time and you can get IDE auto-complete when writing above codes.

## Motivation

If you know JavaScript or Kotlin, you might have written something like this:

```JavaScript
myIterable.forEach((val) => { // do something })

const x = myIterable.map((val) => { // do something })
```

Or some chained calls:

```JavaScript
myIterable.filter((val) => {
  // ...
}).map((val) => {
  // ...
}).forEach((val) => {
  // ...
})
```

But when it comes to C++, things are not so easy. You, as a C++ rising star, may know you can do similar iterations by `#include<algorithm>`.

**But there are no chained calls!**

Inspired by [Kotlin extension functions](https://kotlinlang.org/docs/extensions.html) and the github repo [libciabatta](https://github.com/atomgalaxy/libciabatta),
we can also add "extension functions" to STL containers by mixin.

## Usage

### Add Extensions To An Existing Object

```C++
using Mixin::mix;
using IterablePatch::ForEachIndexed, IterablePatch::Map, IterablePatch::Filter;
using std::cout, std::endl;

struct ObjStyleTest {
    static void run() {
        cout << "Object style syntax: mix things into an object." << endl;
        std::vector<int> a{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        auto b = mix<ForEachIndexed, Map, Filter>::in(a);
        auto c = b.filter([](auto &value) {
            return value % 2 == 0;
        }).map([](auto &value) {
            return value * 10;
        });
        c.for_each_indexed([&c](auto idx, auto &n) {
            cout << n << (idx < c.size() - 1 ? ", " : "");
        });
        cout << endl;

        // Yes, it can be automatically cast back!
        std::vector<int> d = c;
        cout << "Testing implicit cast" << endl;
        bool flag = true;
        c.for_each_indexed([&flag, &d](auto idx, auto &v) {
            flag &= v == d[idx];
        });
        cout << (flag ? "YES!" : "NO!") << endl << endl;
    }
};
```

### Create A Type Alias And Use It To Create Objects

```C++
struct ClassStyleTest {
    template<typename T>
    using vector = typename mix<ForEachIndexed, Map, Filter>::in_class<std::vector<T>>;

    static void run() {
        cout << "Class style syntax: mix things into a class" << endl;
        // Normal constructors and list initializer are all supported.
        // vector<int> a(3, 10);
        // vector<int> a(2);
        vector<int> a{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        auto b = a.filter([](auto &value) {
            return value % 2 == 0;
        }).map([](auto &value) {
            return value * 10;
        });
        b.for_each_indexed([&b](auto idx, auto &value) {
            cout << value << (idx < b.size() - 1 ? ", " : "");
        });
        cout << endl;
        // Yes, it can be automatically cast back!
        std::vector<int> c = b;
        cout << "Testing implicit cast" << endl;
        bool flag = true;
        b.for_each_indexed([&b, &flag](auto idx, auto &value) {
            flag &= value == b[idx];
        });
        cout << (flag ? "YES!" : "NO!") << endl << endl;
    }
};
```


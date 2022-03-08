#include <iostream>
#include <vector>
#include "mixin.h"

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

struct ClassStyleTest {
    template<typename T>
    using vector = typename mix<ForEachIndexed, Map, Filter>::in_class<std::vector<T>>;

    static void run() {
        cout << "Class style syntax: mix things into a class" << endl;
        // Normal constructors and list initializer are all supported.
        vector<int> a{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        // vector<int> a(3, 10);
        // vector<int> a(2);
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

int main() {
    ObjStyleTest::run();
    ClassStyleTest::run();
    return 0;
}

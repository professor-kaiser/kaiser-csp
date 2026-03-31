#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest.h"

#include "../../src/csp/core/structure/ContinuousInterval.hpp"
#include "../../src/csp/core/structure/UnionInterval.hpp"
#include <memory>
#include <vector>
#include <set>
#include <numeric>
#include <iostream>

// g++ -std=c++20 interval.test.cpp ../../src/csp/core/structure/*.cpp -o interval

using namespace kaiser::csp::core::structure;

auto range = [](int begin, int end)
{
    const int N = end - begin;
    std::vector<int> v(std::abs(N));
    std::set<int> s;

    if (N > 0) 
        std::iota(v.begin(), v.end(), begin);
    else 
        std::iota(v.rbegin(), v.rend(), end + 1);

    s.insert(v.begin(), v.end());
    return s;
};

TEST_CASE("[ContinuousInterval]")
{
    ContinuousInterval interval {10, 25};

    CHECK(interval.min() == 10);
    CHECK(interval.max() == 25);
    CHECK(interval.flatten() == range(10, 26));
}

TEST_CASE("[UnionInterval]")
{
    UnionInterval interval({{0, 5}, {10, 35}});

    auto part0 = range(0, 6);
    auto part1 = range(10, 36);
    std::set<int> part;
    part.insert(part0.begin(), part0.end());
    part.insert(part1.begin(), part1.end());

    CHECK(interval.min() == 0);
    CHECK(interval.max() == 35);
    CHECK(interval.flatten() == part);
}
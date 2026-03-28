#ifndef __KAISER_CORE_EXPRESSION_INTEGER_STRUCTURE_HPP__
#define __KAISER_CORE_EXPRESSION_INTEGER_STRUCTURE_HPP__

#include "../structure.hpp"
#include <set>
#include <optional>
#include <vector>
#include <cassert>
#include <algorithm>
#include <cmath>

namespace kaiser::csp::core::expression
{
    namespace structure
    {
        template <>
        struct Bound<int>
        {
            int low, high;

            Bound() = default;

            Bound(int l, int h)
            : low(l), high(h)
            {
                assert(l <= h && "invalid interval");
            }

            Bound<int> operator+(const Bound<int>& other) const { return Bound(low + other.low, high + other.high); }
            
            Bound<int> operator*(double k) const 
            { 
                int left_bound = std::floor(low * k);
                int right_bound = std::ceil(high * k);
                return Bound(left_bound, right_bound); 
            }

            Bound<int>& operator*=(double k) 
            {
                *this = *this * k;
                if (low > high) std::swap(low, high);
                return *this;
            }

            std::optional<Bound<int>> intersect(const Bound<int>& other)
            {
                int bound_low = std::max(other.low, low);
                int bound_high = std::min(other.high, high);

                if (bound_low <= bound_high)
                    return Bound<int>(bound_low, bound_high);

                return std::nullopt;
            }

            std::set<int> to_set() const
            {
                std::set<int> set;
                for (int i = low; i <= high; ++i)
                {
                    set.insert(i);
                }

                return set;
            }

            std::vector<int> to_vector() const
            {
                std::vector<int> vec;
                for (int i = low; i <= high; ++i)
                {
                    vec.push_back(i);
                }

                std::sort(vec.begin(), vec.end());
                return vec;
            }
        };

        std::set<int> to_set(const Bounds<int>& bounds)
        {
            std::set<int> combined;
            for (const auto& b : bounds)
            {
                auto set_bound = b.to_set();
                combined.insert(set_bound.begin(), set_bound.end());
            }

            return combined;
        }

        std::vector<int> to_vector(const Bounds<int>& bounds)
        {
            std::vector<int> combined;
            for (const auto& b : bounds)
            {
                auto vec_bound = b.to_vector();
                combined.insert(combined.end(), vec_bound.begin(), vec_bound.end());
            }

            return combined;
        }

        Bounds<int> to_bounds(const std::set<int>& set)
        {
            Bounds<int> bounds;
            if (set.empty()) return bounds;

            int lo   = *set.begin();
            int prev = lo;

            for (auto it = std::next(set.begin()); it != set.end(); )
            {
                if (*it != prev + 1)
                {
                    bounds.push_back(Bound<int>(lo, prev));
                    lo = *it;
                }
                prev = *it;
            }

            bounds.push_back(Bound<int>(lo, prev));
            return bounds;
        }

    } // namespace structure
    
} // namespace kaiser::csp::core::expression


#endif
#ifndef __KAISER_CORE_EXPRESSION_STRUCTURE_HPP__
#define __KAISER_CORE_EXPRESSION_STRUCTURE_HPP__

#include <set>
#include <optional>

namespace kaiser::csp::core::expression
{
    namespace structure
    {
        template <typename T>
        struct Flat
        {
            std::map<int, T> coeffs;
            T constant;
        };

        template <typename T>
        struct Bound
        {
            T low, high;

            Bound() = default;

            Bound(T l, T h)
            : low(l), high(h)
            {
                assert(l <= h && "invalid interval");
            }

            Bound<T> operator+(const Bound<T>& other) const { return Bound(low + other.low, high + other.high); }
            Bound<T> operator*(double k) const { return Bound(low * k, high * k); }
            Bound<T>& operator*=(double k) 
            {
                low *= k; high *= k;
                if (low > high) std::swap(low, high);
                return *this;
            }

            std::optional<Bound<T>> intersect(const Bound<T>& other)
            {
                T bound_low = std::max(other.low, low);
                T bound_high = std::min(other.high, high);

                if (bound_low <= bound_high)
                    return Bound<T>(bound_low, bound_high);

                return std::nullopt;
            }
        };
        
        template <typename T> using Bounds = std::vector<Bound<T>>;

    } // namespace structure
    
} // namespace kaiser::csp::core::expression


#endif
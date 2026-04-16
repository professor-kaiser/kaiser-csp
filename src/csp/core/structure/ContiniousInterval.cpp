#include "ContinuousInterval.hpp"
#include <numeric>
#include <cmath>

namespace kaiser::csp::core::structure
{
    ContinuousInterval::ContinuousInterval(int min, int max)
    : Interval(min, max)
    {}

    IntervalPtr make_continuous(int min, int max)
    {
        return std::make_shared<ContinuousInterval>(min, max);
    }

    IntervalPtr make_constant(int c)
    {
        return make_continuous(c, c);
    }

    ContinuousInterval operator+(
        const ContinuousInterval& left, 
        const ContinuousInterval& right)
    {
        return ContinuousInterval(
            left.min() + right.min(), 
            left.max() + right.max());
    }

    std::optional<ContinuousInterval> ContinuousInterval::intersect(
        const ContinuousInterval& other)
    {
        int min = std::max(other.min_, min_);
        int max = std::min(other.max_, max_);

        if (min <= max)
            return ContinuousInterval(min, max);

        return std::nullopt;
    }

    IntervalPtr ContinuousInterval::accept(const IntervalAddVisitor& visitor) const
    {
        return visitor.visit(*this);
    }

    IntervalPtr ContinuousInterval::operator*(double k)
    {
        auto result = std::make_shared<ContinuousInterval>(*this);
        (*result) *= k;

        return result;
    }

    Interval& ContinuousInterval::operator*=(double k)
    {
        min_ = std::floor(min_ * k);
        max_ = std::ceil(max_ * k);
        if (min_ > max_) std::swap(min_, max_);

        return *this;
    }

    std::set<int> ContinuousInterval::flatten() const
    {
        std::set<int> flat;
        for (int i = min_; i <= max_; ++i) flat.insert(i);
        return flat;
    }

    IntervalPtr ContinuousInterval::clone() const
    {
        return std::make_shared<ContinuousInterval>(*this);
    }
} // namespace kaiser::csp::core::structure
#include "UnionInterval.hpp"
#include <numeric>
#include <algorithm>
#include <cmath>
#include <set>

namespace kaiser::csp::core::structure
{
    UnionInterval::UnionInterval(std::vector<ContinuousInterval> intervals)
    : Interval(0, 0), continuous_intervals_(std::move(intervals))
    {
        if (!continuous_intervals_.empty())
        {
            std::set<int> flattened = flatten();
            min_ = *std::begin(flattened);
            max_ = *std::rbegin(flattened);
        }
    }

    IntervalPtr make_union(std::vector<ContinuousInterval> ci)
    {
        return std::make_shared<UnionInterval>(std::move(ci));
    }

    UnionInterval operator+(
        const UnionInterval& left, 
        const UnionInterval& right)
    {
        const auto left_ci = left.get_continuous_intervals();
        const auto right_ci = right.get_continuous_intervals();
        const std::size_t N = left_ci.size() + right_ci.size();
        std::vector<ContinuousInterval> ci;
        ci.reserve(N);

        for (const auto& lci : left_ci)
        {
            for (const auto& rci : right_ci)
            {
                ci.push_back(lci + rci);
            }
        }

        return UnionInterval(ci);
    }

    IntervalPtr UnionInterval::accept(const IntervalAddVisitor& visitor) const
    {
        return visitor.visit(*this);
    }

    IntervalPtr UnionInterval::operator*(double k)
    {
        auto result = std::make_shared<UnionInterval>(*this);
        (*result) *= k;
        return result;
    }

    Interval& UnionInterval::operator*=(double k)
    {
        min_ = std::floor(min_ * k);
        max_ = std::ceil(max_ * k);
        if (min_ > max_) std::swap(min_, max_);
        for (ContinuousInterval& ci : continuous_intervals_)
            ci *= k;

        return *this;
    }

    std::set<int> UnionInterval::flatten() const
    {
        std::set<int> flattened;
        for (auto& ci : continuous_intervals_)
        {
            auto flat_ci = ci.flatten();
            flattened.insert(flat_ci.begin(), flat_ci.end());
        }

        return flattened;
    }

    std::vector<ContinuousInterval> UnionInterval::get_continuous_intervals() const
    {
        return continuous_intervals_;
    }
} // namespace kaiser::csp::core::structure
#ifndef __KAISER_CORE_UNION_INTERVAL_HPP__
#define __KAISER_CORE_UNION_INTERVAL_HPP__

#include "Interval.hpp"
#include "ContinuousInterval.hpp"
#include <set>
#include <vector>
#include <memory>

namespace kaiser::csp::core::structure
{
    class UnionInterval : public Interval
    {
    private:
        using Interval::min_;
        using Interval::max_; 
        std::vector<ContinuousInterval> continuous_intervals_;

    public:
        UnionInterval(std::vector<ContinuousInterval>);
        IntervalPtr accept(const IntervalAddVisitor& visitor) const override;
        IntervalPtr operator*(double k) override;
        Interval& operator*=(double k) override;
        std::set<int> flatten() const override;
        UnionInterval intersect(const UnionInterval& other);
        std::vector<ContinuousInterval>& get_continuous_intervals();
        std::vector<ContinuousInterval> get_continuous_intervals() const;
        IntervalPtr clone() const override;
    };

    IntervalPtr make_union(std::vector<ContinuousInterval>);

    UnionInterval operator+(
        const UnionInterval& left, 
        const UnionInterval& right);
} // namespace kaiser::csp::core::structure


#endif
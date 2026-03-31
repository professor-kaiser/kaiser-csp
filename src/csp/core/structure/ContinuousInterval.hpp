#ifndef __KAISER_CORE_STRUCTURE_CONTINUOUS_INTERVAL_HPP__
#define __KAISER_CORE_STRUCTURE_CONTINUOUS_INTERVAL_HPP__

#include "Interval.hpp"
#include <set>
#include <memory>
#include <optional>

namespace kaiser::csp::core::structure
{
    class ContinuousInterval : public Interval
    {
    private:
        using Interval::min_;
        using Interval::max_;

    public:
        ContinuousInterval(int min, int max);
        std::optional<ContinuousInterval> intersect(const ContinuousInterval& other);
        IntervalPtr accept(const IntervalAddVisitor&) const override;
        IntervalPtr operator*(double k) override;
        Interval& operator*=(double k) override;
        std::set<int> flatten() const override;
    };

    IntervalPtr make_continue(int min, int max);

    ContinuousInterval operator+(
        const ContinuousInterval& left, 
        const ContinuousInterval& right);
} // namespace kaiser::csp::core::structure


#endif
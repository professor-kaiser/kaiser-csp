#include "operation.hpp"
#include <algorithm>
#include <stdexcept>
#include <iostream>

namespace kaiser::csp::core::structure
{
    IntervalPtr operator+(const IntervalPtr& left, const IntervalPtr& right)
    {
        struct LeftDispatch : IntervalAddVisitor 
        {
            const IntervalPtr& right;
            explicit LeftDispatch(const IntervalPtr& r) : right(r) {}

            IntervalPtr visit(const ContinuousInterval& l) const override 
            {
                return right->accept(AddWithContinuous{l});
            }

            IntervalPtr visit(const UnionInterval& l) const override 
            {
                return right->accept(AddWithUnion{l});
            }
        };

        return left->accept(LeftDispatch{right});
    }

    IntervalPtr& operator-=(IntervalPtr& interval, int c)
    {
        std::set<int> flat = interval->flatten();
        flat.erase(c);

        interval = std::move(from_set(flat));
        return interval;
    }

    IntervalPtr from_set(const std::set<int>& values)
    {
        if (values.empty())
            return make_union({});

        std::vector<ContinuousInterval> intervals;

        auto it = values.begin();
        int start = *it, last = *it;
        ++it;

        std::for_each(it, values.end(), [&](int v) {
            if (v == last + 1) 
            {
                last = v;
            } 
            else 
            {
                intervals.emplace_back(start, last);
                start = last = v;
            }
        });

        intervals.emplace_back(start, last);

        return make_union(std::move(intervals));
    }

    std::optional<ContinuousInterval> intersect(const ContinuousInterval& ca, const ContinuousInterval& cb)
    {
        int min = std::max(cb.min(), ca.min());
        int max = std::min(cb.max(), ca.max());

        if (min <= max)
            return ContinuousInterval(min, max);

        return std::nullopt;
    }

    UnionInterval intersect(const UnionInterval& ua, const UnionInterval& ub)
    {
        std::vector<ContinuousInterval> ci;
        auto ua_ci = ua.get_continuous_intervals();
        auto ub_ci = ub.get_continuous_intervals();

        auto it_a = ua_ci.begin();
        auto it_b = ub_ci.begin();

        while (it_a != ua_ci.end() && it_b != ub_ci.end())
        {
            auto inter = it_a->intersect(*it_b);
            if (inter.has_value())
                ci.push_back(inter.value());

            if (it_a->max() < it_b->max()) ++it_a;
            else ++it_b;
        }

        return UnionInterval(std::move(ci));
    }

    UnionInterval intersect(const UnionInterval& u, const ContinuousInterval& c)
    {
        return intersect(u, UnionInterval({c}));
    }

    UnionInterval intersect(const ContinuousInterval& c, const UnionInterval& u)
    {
        return intersect(u, c);
    }

    IntervalPtr intersect(const IntervalPtr& a, const IntervalPtr& b)
    {
        auto ua = std::dynamic_pointer_cast<UnionInterval>(a);
        auto ub = std::dynamic_pointer_cast<UnionInterval>(b);
        auto ca = std::dynamic_pointer_cast<ContinuousInterval>(a);
        auto cb = std::dynamic_pointer_cast<ContinuousInterval>(b);

        if (ua && ub) return std::make_shared<UnionInterval>(intersect(*ua, *ub));
        if (ua && cb) return std::make_shared<UnionInterval>(intersect(*ua, *cb));
        if (ca && ub) return std::make_shared<UnionInterval>(intersect(*ca, *ub));
        if (ca && cb) 
        {
            auto r = intersect(*ca, *cb);
            if (r.has_value()) return std::make_shared<ContinuousInterval>(r.value());
            return std::make_shared<UnionInterval>(UnionInterval({}));
        }

        throw std::runtime_error("intersect: unknown Interval type");
    }
    
    // IntervalPtr operator*(IntervalPtr interval, double k)
    // {
    //     return (*interval) * k;
    // }

    // IntervalPtr operator*=(IntervalPtr interval, double k)
    // {
    //     (*interval) *= k;
    //     return interval;
    // }
} // namespace kaiser::csp::core::structure
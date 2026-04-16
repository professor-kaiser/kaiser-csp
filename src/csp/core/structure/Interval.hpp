#ifndef __KAISER_CORE_STRUCTURE_INTERVAL_HPP__
#define __KAISER_CORE_STRUCTURE_INTERVAL_HPP__

#include <set>
#include <cassert>
#include <memory>
#include "IntervalAddVisitor.hpp"

namespace kaiser::csp::core::structure
{
    class AddWithContinue;
    class AddWithUnion;
    class Interval;
    using IntervalPtr = std::shared_ptr<Interval>;

    class Interval : public std::enable_shared_from_this<Interval>
    {
    protected:
        int min_, max_;    

    public:
        Interval(int min, int max) : min_(min), max_(max) 
        {
            assert(min <= max && "Invalid interval");
        }

        virtual int min() const { return min_; }
        virtual int max() const { return max_; }
        virtual IntervalPtr accept(const IntervalAddVisitor&) const = 0;
        virtual IntervalPtr operator*(double k) = 0;
        virtual Interval& operator*=(double k) = 0;
        virtual std::set<int> flatten() const = 0;
        virtual IntervalPtr clone() const = 0;
        virtual ~Interval() = default;
    };
} // namespace kaiser::csp::core::structure


#endif
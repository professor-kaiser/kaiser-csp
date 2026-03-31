#ifndef __KAISER_CORE_STRUCTURE_INTERVAL_ADD_VISITOR_HPP__
#define __KAISER_CORE_STRUCTURE_INTERVAL_ADD_VISITOR_HPP__

#include <memory>

namespace kaiser::csp::core::structure
{
    class Interval;
    class ContinuousInterval;
    class UnionInterval;
    using IntervalPtr = std::shared_ptr<Interval>;

    struct IntervalAddVisitor
    {
        virtual IntervalPtr visit(const ContinuousInterval&) const = 0;
        virtual IntervalPtr visit(const UnionInterval&) const = 0; 
        virtual ~IntervalAddVisitor() = default;
    };
}

#endif
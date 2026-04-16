#ifndef __KAISER_CORE_STRUCTURE_OPERATION_HPP__
#define __KAISER_CORE_STRUCTURE_OPERATION_HPP__

#include "Interval.hpp"
#include "ContinuousInterval.hpp"
#include "UnionInterval.hpp"
#include "IntervalAddVisitor.hpp"
#include "AddWithContinuous.hpp"
#include "AddWithUnion.hpp"
#include <memory>
#include <optional>

namespace kaiser::csp::core::structure
{
    IntervalPtr operator+(const IntervalPtr& left, const IntervalPtr& right);
    IntervalPtr from_set(const std::set<int>&);
    IntervalPtr& operator-=(IntervalPtr&, int);

    std::optional<ContinuousInterval> intersect(const ContinuousInterval& ca, const ContinuousInterval& cb);
    UnionInterval intersect(const UnionInterval& ca, const UnionInterval& cb);
    UnionInterval intersect(const UnionInterval&, const ContinuousInterval&);
    UnionInterval intersect(const ContinuousInterval&, const UnionInterval&);
    IntervalPtr intersect(const IntervalPtr&, const IntervalPtr&);
    // IntervalPtr operator*(IntervalPtr interval, double k);
    // IntervalPtr operator*=(IntervalPtr interval, double k);
} // namespace kaiser::csp::core::structure

#endif
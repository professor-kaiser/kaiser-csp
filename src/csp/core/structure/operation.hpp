#ifndef __KAISER_CORE_STRUCTURE_OPERATION_HPP__
#define __KAISER_CORE_STRUCTURE_OPERATION_HPP__

#include "Interval.hpp"
#include "ContinuousInterval.hpp"
#include "UnionInterval.hpp"
#include "IntervalAddVisitor.hpp"
#include "AddWithContinuous.hpp"
#include "AddWithUnion.hpp"
#include <memory>

namespace kaiser::csp::core::structure
{
    IntervalPtr operator+(const IntervalPtr& left, const IntervalPtr& right);
    // IntervalPtr operator*(IntervalPtr interval, double k);
    // IntervalPtr operator*=(IntervalPtr interval, double k);
} // namespace kaiser::csp::core::structure

#endif
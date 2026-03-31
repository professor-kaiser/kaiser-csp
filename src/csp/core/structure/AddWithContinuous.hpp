#ifndef __KAISER_CORE_STRUCTURE_INTERVAL_ADD_WITH_CONTINUOUS_HPP__
#define __KAISER_CORE_STRUCTURE_INTERVAL_ADD_WITH_CONTINUOUS_HPP__

#include <memory>
#include "IntervalAddVisitor.hpp"
#include "ContinuousInterval.hpp"
#include "UnionInterval.hpp"

namespace kaiser::csp::core::structure
{
    struct AddWithContinuous : public IntervalAddVisitor
    {
        const ContinuousInterval& left;

        explicit AddWithContinuous(const ContinuousInterval& l);
        IntervalPtr visit(const ContinuousInterval&) const override;
        IntervalPtr visit(const UnionInterval&) const override;
    };
}

#endif
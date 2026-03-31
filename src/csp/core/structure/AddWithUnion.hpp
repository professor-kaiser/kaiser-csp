#ifndef __KAISER_CORE_STRUCTURE_INTERVAL_ADD_WITH_UNION_HPP__
#define __KAISER_CORE_STRUCTURE_INTERVAL_ADD_WITH_UNION_HPP__

#include <memory>
#include "IntervalAddVisitor.hpp"
#include "ContinuousInterval.hpp"
#include "UnionInterval.hpp"

namespace kaiser::csp::core::structure
{
    struct AddWithUnion : public IntervalAddVisitor
    {
        const UnionInterval& left;

        explicit AddWithUnion(const UnionInterval& l);
        IntervalPtr visit(const ContinuousInterval&) const override;
        IntervalPtr visit(const UnionInterval&) const override;
    };
}

#endif
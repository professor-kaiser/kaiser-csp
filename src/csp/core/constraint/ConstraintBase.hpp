#ifndef __KAISER_CORE_CONSTRAINT_CONSTRAINT_BASE_HPP__
#define __KAISER_CORE_CONSTRAINT_CONSTRAINT_BASE_HPP__

#include "../expressions/ExpressionBase.hpp"
#include <vector>
#include <memory>

using namespace kaiser::csp::core::expression;

namespace kaiser::csp::core::constraint
{
    struct ConstraintBase;
    using Constraint = std::shared_ptr<ConstraintBase>;

    struct ConstraintBase
    {
        Flat left, right;

        ConstraintBase(Flat l, Flat r)
        : left(l), right(r)
        {}

        virtual bool compare(const std::vector<int>&) = 0;
        virtual ~ConstraintBase() = default;
    };
    
} // namespace kaiser::csp::core::constraint


#endif
#ifndef __KAISER_CORE_CONSTRAINT_EQUAL_CONSTRAINT_BASE_HPP__
#define __KAISER_CORE_CONSTRAINT_EQUAL_CONSTRAINT_BASE_HPP__

#include "ConstraintBase.hpp"

using namespace kaiser::csp::core::expression;

namespace kaiser::csp::core::constraint
{
    struct EqualConstraint : ConstraintBase
    {
        using ConstraintBase::left;
        using ConstraintBase::right;

        EqualConstraint(Flat l, Flat r);
        bool compare(const std::vector<int>&) override;
    };
    
    Constraint operator==(Flat left, Flat right);
} // namespace kaiser::csp::core::constraint


#endif
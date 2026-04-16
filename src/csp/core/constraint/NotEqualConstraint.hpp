#ifndef __KAISER_CORE_CONSTRAINT_NOT_EQUAL_CONSTRAINT_BASE_HPP__
#define __KAISER_CORE_CONSTRAINT_NOT_EQUAL_CONSTRAINT_BASE_HPP__

#include "ConstraintBase.hpp"

using namespace kaiser::csp::core::expression;

namespace kaiser::csp::core::constraint
{
    struct NotEqualConstraint : public ConstraintBase
    {
        using ConstraintBase::context;
        using ConstraintBase::left_id;
        using ConstraintBase::right_id;
        Flat cached_diff;

        NotEqualConstraint(std::string ctx, int l, int r);
        void prepare() override;
        bool compare(std::span<const int>) override;
    };
} // namespace kaiser::csp::core::constraint


#endif
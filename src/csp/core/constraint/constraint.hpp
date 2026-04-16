#ifndef __KAISER_CORE_CONSTRAINT_CONSTRAINT_FUNCTION_HPP__
#define __KAISER_CORE_CONSTRAINT_CONSTRAINT_FUNCTION_HPP__

#include "ConstraintBase.hpp"
#include "EqualConstraint.hpp"
#include "NotEqualConstraint.hpp"
#include "../expressions/ExpressionBase.hpp"
#include "../registry/Registry.hpp"
#include <memory>

using namespace kaiser::csp::core::expression;
using namespace kaiser::csp::core::registry;

namespace kaiser::csp::core::constraint
{
    Constraint equal(std::string ctx, int left, int right);
    Constraint equal(const Expression& left, const Expression& right);

    Constraint not_equal(const Expression& left, const Expression& right);
}

#endif
#ifndef __KAISER_CORE_EXPRESSION_INTEGER_SCALE_EXPRESSION__HPP__
#define __KAISER_CORE_EXPRESSION_INTEGER_SCALE_EXPRESSION__HPP__

#include "ExpressionBase.hpp"
#include <memory>
#include <vector>
#include <cmath>

namespace kaiser::csp::core::expression
{
    struct ScaleExpression : public ExpressionBase
    {
        using ExpressionPtr = std::shared_ptr<ExpressionBase>;

        using ExpressionBase::domains;
        ExpressionPtr expression;
        int scalar;

        ScaleExpression(ExpressionPtr expr, int s);
        void propagate_domains(IntervalPtr interval) override;
        Flat flatten() const override;
    };
    
} // namespace kaiser::csp::core::expression


#endif
#ifndef __KAISER_CORE_EXPRESSIONS_LINEAR_EXPRESSION_HPP__
#define __KAISER_CORE_EXPRESSIONS_LINEAR_EXPRESSION_HPP__

#include "ExpressionBase.hpp"
#include <vector>

namespace kaiser::csp::core::expression
{
    struct LinearExpression : public ExpressionBase
    {
        using ExpressionBase::domains;
        std::map<int, int> coeffs;
        int constant;

        LinearExpression(IntervalPtr interval, std::map<int, int> c = {}, int k = 0);
        LinearExpression(IntervalPtr interval, int idx, int coeff = 1);
        void propagate_domains(IntervalPtr interval) override;
        Flat flatten() const override;
    };
    
} // namespace kaiser::csp::core::expression


#endif
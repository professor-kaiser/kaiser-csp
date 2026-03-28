#ifndef __KAISER_CORE_EXPRESSION_INTEGER_LINEAR_EXPRESSION_HPP__
#define __KAISER_CORE_EXPRESSION_INTEGER_LINEAR_EXPRESSION_HPP__

#include "../ExpressionBase.hpp"
#include "structure.hpp"
#include "../LinearExpression.hpp"
#include <vector>

namespace kaiser::csp::core::expression
{

    template <>
    struct LinearExpression<int> : public ExpressionBase<int>
    {
        using ExpressionBase<int>::domains;
        std::map<int, int> coeffs;
        int constant;

        LinearExpression(Bounds<int> bounds, std::map<int, int> c = {}, int k = 0)
        : ExpressionBase<int>(bounds), coeffs(std::move(c)), constant(k)
        {}

        LinearExpression(Bounds<int> bounds, int idx, int coeff = 1)
        : ExpressionBase<int>(bounds), coeffs({{idx, coeff}}), constant(0)
        {}

        void propagate_domains(Bounds<int> bounds) override
        {
            domains = std::move(bounds);
        }
        
        Flat<int> flatten() const override
        {
            return {coeffs, constant};
        }
    };
    
} // namespace kaiser::csp::core::expression


#endif
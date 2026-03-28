#ifndef __KAISER_CORE_EXPRESSION_LINEAR_EXPRESSION_HPP__
#define __KAISER_CORE_EXPRESSION_LINEAR_EXPRESSION_HPP__

#include "ExpressionBase.hpp"
#include <vector>

namespace kaiser::csp::core::expression
{

    template <typename T>
    struct LinearExpression : public ExpressionBase<T>
    {
        using ExpressionBase<T>::domains;
        std::map<int, T> coeffs;
        T constant;

        LinearExpression(Bounds<T> bounds, std::map<int, T> c = {}, T k = 0)
        : ExpressionBase<T>(bounds), coeffs(std::move(c)), constant(k)
        {}

        LinearExpression(Bounds<T> bounds, int idx, T coeff = 1)
        : ExpressionBase<T>(bounds), coeffs({{idx, coeff}}), constant(0)
        {}

        void propagate_domains(Bounds<T> bounds) override
        {
        }
        
        Flat<T> flatten() const override
        {
            return {coeffs, constant};
        }
    };
    
} // namespace kaiser::csp::core::expression


#endif
#ifndef __KAISER_CORE_EXPRESSION_SCALE_EXPRESSION__HPP__
#define __KAISER_CORE_EXPRESSION_SCALE_EXPRESSION__HPP__

#include "ExpressionBase.hpp"
#include <memory>
#include <iostream>
#include <vector>

namespace kaiser::csp::core::expression
{
    template <typename T>
    struct ScaleExpression : public ExpressionBase<T>
    {
        using ExpressionPtr = std::shared_ptr<ExpressionBase<T>>;

        using ExpressionBase<T>::domains;
        ExpressionPtr expression;
        T scalar;

        ScaleExpression(ExpressionPtr expr, T s)
        : ExpressionBase<T>(expr->domains), expression(std::move(expr)), scalar(s)
        {
            for (auto& bound : domains)
                bound *= s;
        }

        void propagate_domains(Bounds<T> bounds) override
        {
        }

        Flat<T> flatten() const override
        {
            auto [coeffs, k] = expression->flatten();

            for (auto& [idx, coeff] : coeffs)
                coeff *= scalar;

            return {coeffs, k*scalar};
        }
    };
    
} // namespace kaiser::csp::core::expression


#endif
#ifndef __KAISER_CORE_EXPRESSION_INTEGER_SCALE_EXPRESSION__HPP__
#define __KAISER_CORE_EXPRESSION_INTEGER_SCALE_EXPRESSION__HPP__

#include "../ExpressionBase.hpp"
#include "../ScaleExpression.hpp"
#include "structure.hpp"
#include <memory>
#include <iostream>
#include <vector>
#include <cmath>

namespace kaiser::csp::core::expression
{
    template <>
    struct ScaleExpression<int> : public ExpressionBase<int>
    {
        using ExpressionPtr = std::shared_ptr<ExpressionBase<int>>;

        using ExpressionBase<int>::domains;
        ExpressionPtr expression;
        int scalar;

        ScaleExpression(ExpressionPtr expr, int s)
        : ExpressionBase<int>(expr->domains), expression(std::move(expr)), scalar(s)
        {
            for (auto& bound : domains)
                bound *= s;
        }

        void propagate_domains(Bounds<int> bounds) override
        {
            domains = std::move(bounds);
            if (scalar == 0) return;

            Bounds<int> expr_bounds(domains.size());
            for (size_t i = 0; i < domains.size(); ++i)
            {
                expr_bounds[i] = domains[i] * (1.0/scalar);
            }

            expression->propagate_domains(std::move(expr_bounds));
        }

        Flat<int> flatten() const override
        {
            auto [coeffs, k] = expression->flatten();

            for (auto& [idx, coeff] : coeffs)
                coeff *= scalar;

            return {coeffs, k*scalar};
        }
    };
    
} // namespace kaiser::csp::core::expression


#endif
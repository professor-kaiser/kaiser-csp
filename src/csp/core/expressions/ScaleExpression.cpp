#include "ScaleExpression.hpp"
#include "../structure/operation.hpp"

namespace kaiser::csp::core::expression
{
    ScaleExpression::ScaleExpression(std::string context, ExpressionPtr expr, int s)
    : ExpressionBase(std::move(context), std::move((*expr->domains) * s)),
      expression(std::move(expr)), scalar(s)
    {
    }

    ScaleExpression::ScaleExpression(ExpressionPtr expr, int s)
    : ExpressionBase(std::move(expr->get_context()), std::move((*expr->domains) * s)),
      expression(std::move(expr)), scalar(s)
    {
    }

    void ScaleExpression::propagate_domains(IntervalPtr interval)
    {
        //domains = std::move(interval);
        *domains = *intersect(domains, interval);
        update_domains();

        if (scalar == 0) return;

        IntervalPtr expr_domains = (*domains) * (1.0/scalar);
        expression->propagate_domains(std::move(expr_domains));
    }

    Flat ScaleExpression::flatten() const
    {
        auto [coeffs, k] = expression->flatten();

        for (auto& [idx, coeff] : coeffs)
            coeff *= scalar;

        return {coeffs, k*scalar};
    }
}
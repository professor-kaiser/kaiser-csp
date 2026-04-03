#include "LinearExpression.hpp"

namespace kaiser::csp::core::expression
{
    LinearExpression::LinearExpression(IntervalPtr interval, std::map<int, int> c, int k)
    : ExpressionBase(interval), coeffs(std::move(c)), constant(k)
    {
    }

    LinearExpression::LinearExpression(IntervalPtr interval, int idx, int coeff)
    : ExpressionBase(interval), coeffs({{idx, coeff}}), constant(0)
    {}

    void LinearExpression::propagate_domains(IntervalPtr interval)
    {
        domains = std::move(interval);
    }
    
    Flat LinearExpression::flatten() const
    {
        return {coeffs, constant};
    }
}
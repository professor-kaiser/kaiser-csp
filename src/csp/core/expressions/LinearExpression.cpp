#include "LinearExpression.hpp"
#include "../structure/operation.hpp"

namespace kaiser::csp::core::expression
{
    LinearExpression::LinearExpression(IntervalPtr interval, std::map<int, int> c, int k)
    : ExpressionBase(interval), coeffs(std::move(c)), constant(k)
    {
    }

    LinearExpression::LinearExpression(IntervalPtr interval, int idx, int coeff)
    : ExpressionBase(interval), coeffs({{idx, coeff}}), constant(0)
    {}

    LinearExpression::LinearExpression(std::string context, IntervalPtr interval, std::map<int, int> c, int k)
    : ExpressionBase(std::move(context), std::move(interval)), coeffs(std::move(c)), constant(k)
    {
    }

    LinearExpression::LinearExpression(std::string context, IntervalPtr interval, int idx, int coeff)
    : ExpressionBase(std::move(context), std::move(interval)), coeffs({{idx, coeff}}), constant(0)
    {}

    void LinearExpression::propagate_domains(IntervalPtr interval)
    {
        //domains = std::move(interval);
        *domains = *intersect(domains, interval);
        update_domains();
    }
    
    Flat LinearExpression::flatten() const
    {
        return {coeffs, constant};
    }
}
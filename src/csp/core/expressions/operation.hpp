#ifndef __KAISER_CORE_EXPRESSION_OPERATION_HPP__
#define __KAISER_CORE_EXPRESSION_OPERATION_HPP__

#include <map>
#include <array>
#include <memory>
#include "ExpressionBase.hpp"
#include "LinearExpression.hpp"
#include "SumExpression.hpp"
#include "ScaleExpression.hpp"

namespace kaiser::csp::core::expression
{
    /// SUM
    template <typename LHS, typename RHS>
    auto operator+(
        std::shared_ptr<LHS> l,
        std::shared_ptr<RHS> r)
    {
        return make_expression<SumExpression>(std::move(l), std::move(r));
    }

    template <typename Derived>
    std::shared_ptr<ExpressionBase> operator+(
        const std::shared_ptr<Derived>& expr, int k)
    {
        return make_expression<SumExpression>(
            expr,
            make_expression<LinearExpression>(
                std::make_shared<ContinuousInterval>(k, k),
                std::map<int, int>{}, 
                k)
        );
    }

    template <typename Derived>
    std::shared_ptr<ExpressionBase> operator+(
        int k, const std::shared_ptr<Derived>& expr)
    {
        return expr + k;
    }

    template <typename LHS, typename RHS>
    auto operator-(
        std::shared_ptr<LHS> l,
        std::shared_ptr<RHS> r)
    {
        return make_expression<SumExpression>(
            std::move(l), 
            make_expression<ScaleExpression>(std::move(r), -1)
        );
    }

    template <typename Derived>
    std::shared_ptr<ExpressionBase> operator-(
        const std::shared_ptr<Derived>& expr, int k)
    {
        
        return expr + (-k);
    }

    template <typename Derived>
    auto operator-(const std::shared_ptr<Derived>& expr)
    {
        return expr * (-1);
    }

    /// SCALE
    template <typename Derived>
    std::shared_ptr<ExpressionBase> operator*(
        const std::shared_ptr<Derived>& expr, int k)
    {
        return make_expression<ScaleExpression>(expr, k);
    }

    template <typename Derived>
    std::shared_ptr<ExpressionBase> operator*(
        int k, const std::shared_ptr<Derived>& expr)
    {
        return expr * k;
    }
} // namespace kaiser::csp::core::expression


#endif
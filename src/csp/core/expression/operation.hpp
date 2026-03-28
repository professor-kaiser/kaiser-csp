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
        using T = typename LHS::value_type;
        return std::make_shared<SumExpression<T>>(std::move(l), std::move(r));
    }

    template <typename T, typename Derived>
    std::shared_ptr<ExpressionBase<T>> operator+(
        const std::shared_ptr<Derived>& expr, T k)
    {
        return std::make_shared<SumExpression<T>>(
            expr,
            std::make_shared<LinearExpression<T>>(Bounds<T>{{k, k}}, std::map<int, T>{}, k)
        );
    }

    template <typename T, typename Derived>
    std::shared_ptr<ExpressionBase<T>> operator+(
        T k, const std::shared_ptr<Derived>& expr)
    {
        return expr + k;
    }

    template <typename LHS, typename RHS>
    auto operator-(
        std::shared_ptr<LHS> l,
        std::shared_ptr<RHS> r)
    {
        using T = typename LHS::value_type;
        return std::make_shared<SumExpression<T>>(
            std::move(l), 
            std::make_shared<ScaleExpression<T>>(std::move(r), static_cast<T>(-1))
        );
    }

    template <typename T, typename Derived>
    std::shared_ptr<ExpressionBase<T>> operator-(
        const std::shared_ptr<Derived>& expr, T k)
    {
        
        return expr + (-k);
    }

    template <typename Derived>
    auto operator-(const std::shared_ptr<Derived>& expr)
    {
        using T = typename Derived::value_type;
        return expr * static_cast<T>(-1);
    }

    /// SCALE
    template <typename T, typename Derived>
    std::shared_ptr<ExpressionBase<T>> operator*(
        const std::shared_ptr<Derived>& expr, T k)
    {
        return std::make_shared<ScaleExpression<T>>(expr, k);
    }

    template <typename T, typename Derived>
    std::shared_ptr<ExpressionBase<T>> operator*(
        T k, const std::shared_ptr<Derived>& expr)
    {
        return expr * k;
    }
} // namespace kaiser::csp::core::expression


#endif
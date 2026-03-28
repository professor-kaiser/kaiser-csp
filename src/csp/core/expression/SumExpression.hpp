#ifndef __KAISER_CORE_EXPRESSION_SUM_EXPRESSION__HPP__
#define __KAISER_CORE_EXPRESSION_SUM_EXPRESSION__HPP__

#include "ExpressionBase.hpp"
#include "LinearExpression.hpp"
#include <memory>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <iostream>

namespace kaiser::csp::core::expression
{
    template <typename T>
    struct SumExpression : public ExpressionBase<T>
    {
        using ExpressionPtr = std::shared_ptr<ExpressionBase<T>>;

        using ExpressionBase<T>::domains;
        ExpressionPtr left, right;

        SumExpression(ExpressionPtr l_expr, ExpressionPtr r_expr)
        : ExpressionBase<T>({}),
          left(std::move(l_expr)), right(std::move(r_expr))
        {
            for (const auto& l_bound : left->domains)
            {
                for (const auto& r_bound : right->domains)
                {
                    domains.push_back(l_bound + r_bound);
                }
            }
        }

        void propagate_domains(Bounds<T> bounds) override
        {
        }

        Flat<T> flatten() const override
        {
            auto [l_coeffs, lk] = left->flatten();
            auto [r_coeffs, rk] = right->flatten();

            for (auto& [idx, rc] : r_coeffs)
                l_coeffs[idx] += rc;

            for (auto it = l_coeffs.begin(); it != l_coeffs.end(); )
                it->second == 0.0 ? it = l_coeffs.erase(it) : ++it;

            return {l_coeffs, lk + rk};
        }
    };
    
} // namespace kaiser::csp::core::expression


#endif
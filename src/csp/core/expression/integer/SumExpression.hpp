#ifndef __KAISER_CORE_EXPRESSION_INTEGER_SUM_EXPRESSION__HPP__
#define __KAISER_CORE_EXPRESSION_INTEGER_SUM_EXPRESSION__HPP__

#include "../ExpressionBase.hpp"
#include "../LinearExpression.hpp"
#include "../SumExpression.hpp"
#include "structure.hpp"
#include <memory>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <iostream>

namespace kaiser::csp::core::expression
{
    template <>
    struct SumExpression<int> : public ExpressionBase<int>
    {
        using ExpressionPtr = std::shared_ptr<ExpressionBase<int>>;

        using ExpressionBase<int>::domains;
        ExpressionPtr left, right;

        SumExpression(ExpressionPtr l_expr, ExpressionPtr r_expr)
        : ExpressionBase<int>({}),
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

        void propagate_domains(Bounds<int> bounds) override
        {
            domains = std::move(bounds);
            std::vector<int> parent_domains = to_vector(domains);
            std::vector<int> left_domains = to_vector(left->domains);
            std::vector<int> right_domains = to_vector(right->domains);

            // PARENT
            const int parent_min = *std::min_element(parent_domains.begin(), parent_domains.end());
            const int parent_max = *std::max_element(parent_domains.begin(), parent_domains.end());
            // LEFT
            const int left_min = *std::min_element(left_domains.begin(), left_domains.end());
            const int left_max = *std::max_element(left_domains.begin(), left_domains.end());
            // RIGHT
            const int right_min = *std::min_element(right_domains.begin(), right_domains.end());
            const int right_max = *std::max_element(right_domains.begin(), right_domains.end());

            auto left_bound = Bound<int>(parent_min - right_max, parent_max - right_min)
                .intersect(Bound<int>(left_min, left_max));

            auto right_bound = Bound<int>(parent_min - left_max, parent_max - left_max)
                .intersect(Bound<int>(right_min, right_max));

            Bounds<int> node_bounds;
            node_bounds.reserve(1);

            if (left_bound)
                node_bounds.emplace_back(left_bound.value());

            left->propagate_domains(node_bounds);
            
            node_bounds.clear();

            if (right_bound)
                node_bounds.emplace_back(right_bound.value());
                
            right->propagate_domains(node_bounds);
        }

        Flat<int> flatten() const override
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
#ifndef __KAISER_CORE_EXPRESSION_INTEGER_SUM_EXPRESSION__HPP__
#define __KAISER_CORE_EXPRESSION_INTEGER_SUM_EXPRESSION__HPP__

#include "ExpressionBase.hpp"
#include "../structure/operation.hpp"
#include <memory>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <iostream>

namespace kaiser::csp::core::expression
{
    struct SumExpression : public ExpressionBase
    {
        using ExpressionPtr = std::shared_ptr<ExpressionBase>;

        using ExpressionBase::domains;
        using ExpressionBase::id;
        ExpressionPtr left, right;

        SumExpression(ExpressionPtr l_expr, ExpressionPtr r_expr)
        : ExpressionBase(nullptr),
          left(std::move(l_expr)), right(std::move(r_expr))
        {
            domains = left->domains + right->domains;
            // for (const auto& l_bound : left->domains)
            // {
            //     for (const auto& r_bound : right->domains)
            //     {
            //         domains.push_back(l_bound + r_bound);
            //     }
            // }
        }

        void propagate_domains(IntervalPtr intervals) override
        {
            domains = std::move(intervals);
            auto parent_domains = domains->flatten();
            auto left_domains = left->domains->flatten();
            auto right_domains = right->domains->flatten();

            // PARENT
            const int parent_min = *std::begin(parent_domains);
            const int parent_max = *std::rbegin(parent_domains);
            // LEFT
            const int left_min = *std::begin(left_domains);
            const int left_max = *std::rbegin(left_domains);
            // RIGHT
            const int right_min = *std::begin(right_domains);
            const int right_max = *std::rbegin(right_domains);

            auto left_interval = ContinuousInterval(parent_min - right_max, parent_max - right_min)
                .intersect(ContinuousInterval(left_min, left_max));
                
            auto right_interval = ContinuousInterval(parent_min - left_max, parent_max - left_min)
                .intersect(ContinuousInterval(right_min, right_max));

            std::vector<ContinuousInterval> prop_interval;

            if (left_interval)
                prop_interval.push_back(left_interval.value());
            
            left->propagate_domains(std::make_shared<UnionInterval>(prop_interval));

            prop_interval.clear();

            if (right_interval)
                prop_interval.push_back(right_interval.value());

            right->propagate_domains(std::make_shared<UnionInterval>(prop_interval));
        }

        Flat flatten() const override
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
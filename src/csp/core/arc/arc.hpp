#ifndef __KAISER_CORE_ARC_HPP__
#define __KAISER_CORE_ARC_HPP__

#include "../expressions/ExpressionBase.hpp"
#include "../constraint/ConstraintBase.hpp"
#include "../structure/Interval.hpp"
#include "../structure/operation.hpp"
#include "../registry/ContextGuard.hpp"
#include "../registry/ContextGuardComposite.hpp"
#include <functional>
#include <set>
#include <queue>
#include <tuple>
#include <algorithm>
#include <memory>

using namespace kaiser::csp::core::expression;
using namespace kaiser::csp::core::constraint;
using namespace kaiser::csp::core::structure;

namespace kaiser::csp::core::arc
{
    inline bool revise(
        std::set<int>& x_set, 
        const std::set<int>& y_set, 
        const std::function<bool(int, int)>& constraint)
    {
        auto old_size = x_set.size();

        std::erase_if(x_set, [&](const int& x) {
            return std::none_of(y_set.begin(), y_set.end(), [&](const int& y) {
                return constraint(x, y);
            });
        });

        return x_set.size() != old_size;
    }

    inline bool ac3(std::string context)
    {
        std::set<int> need_propagation;
        std::queue<std::pair<int, int>> q;

        auto& neig_reg = NeighborsRegistry::instance();
        auto& expr_reg = ExpressionRegistry::instance();
        auto& revise_reg = ReviseRegistry::instance();

        ContextGuardComposite guards;
        guards.add(make_context_guard(neig_reg, context))
              .add(make_context_guard(expr_reg, context))
              .add(make_context_guard(revise_reg, context));

        for (const auto [id, vec] : neig_reg.get_registry())
            for (const auto v : vec) q.push({id, v});

        while (!q.empty())
        {
            auto [xi, xj] = q.front();
            q.pop();
            
            auto& expr_xi = expr_reg.at(xi);
            auto& expr_xj = expr_reg.at(xj);

            auto& revise_fn = revise_reg.at(make_key(xi, xj));

            if (revise_fn(expr_xi->domains, expr_xj->domains))
            {
                const auto& xi_flat_doms = expr_xi->domains->flatten();
                if (xi_flat_doms.empty()) return false;

                for (int xk : neig_reg.at(xi))
                    if (xk != xj) q.push(std::make_pair(xk, xi));

                need_propagation.insert(xi);
            }
        }

        for (int xi : need_propagation)
        {
            auto& expr = expr_reg.at(xi);
            expr->propagate_domains(expr->domains);
        }
        
        return true;
    }

    /*
    inline bool ac4(std::string context)
    {
        auto& constraint_reg = ConstraintRegistry::instance();
        auto& neighbors_reg = NeighborsRegistry::instance();
        auto& expression_reg = ExpressionRegistry::instance();

        ContextGuardComposite guards;
        guards.add(make_context_guard(constraint_reg, context))
              .add(make_context_guard(neighbors_reg, context))
              .add(make_context_guard(expression_reg, context));

        std::queue<std::pair<int, int>> q;
        std::map<std::tuple<int, int, int>, int> supports;
        std::map<int, std::set<int>> domains;

        auto count_support = [&] (int xi, int xj, int vi) -> int
        {
            int count = 0;

            if (domains.find(xj) == domains.end())
                domains[xj] = expression_reg.at(xj)->domains->flatten();

            for (int vj : domains[xj])
            {
                auto constraint_func = constraint_reg.at(make_key(xi, xj));
                if (constraint_func(vi, vj)) count++;
            }

            return count;
        };

        const auto neighbors = neighbors_reg.get_registry();
        for (const auto [xi, neig_xi] : neighbors)
        {
            for (int xj : neig_xi)
            {
                // const std::set<int> domain_xi = expression_reg.at(xi)->domains->flatten();
                if (domains.find(xi) == domains.end())
                    domains[xi] = expression_reg.at(xi)->domains->flatten();

                for (int vi : domains[xi])
                {
                    supports[{xi, xj, vi}] = count_support(xi, xj, vi);
                    if (supports[{xi, xj, vi}] == 0)
                    {
                        q.push({xi, vi});
                    }
                }
            }
        }

        while (!q.empty())
        {
            auto [xi, vi] = q.front();
            q.pop();

            if (domains[xi].find(vi) == domains[xi].end()) continue;

            domains[xi].erase(vi);
            if (domains[xi].empty())
                return false;

            for (int xk : neighbors.at(xi))
            {
                for (int vk : domains.at(xk))
                {
                    if (vk == xk) continue;

                    std::tuple<int, int, int> key = std::make_tuple(xk, xi, vk);
                    if (supports.count(key))
                    {
                        supports[key]--;

                        if (supports[key] == 0)
                            q.push({xk, vk});
                    }
                }
            }
        }

        return true;
    }
    */
    inline bool ac4(std::string context)
    {
        auto& neighbors_reg  = NeighborsRegistry::instance();
        auto& expression_reg = ExpressionRegistry::instance();
        auto& revise_reg     = ReviseRegistry::instance();

        ContextGuardComposite guards;
        guards.add(make_context_guard(neighbors_reg,  context))
              .add(make_context_guard(expression_reg, context))
              .add(make_context_guard(revise_reg,     context));

        const auto& neighbors = neighbors_reg.get_registry();

        // Snapshot des domaines (IntervalPtr, pas flatten)
        std::unordered_map<int, IntervalPtr> domains;
        domains.reserve(neighbors.size());
        for (const auto& [xi, _] : neighbors)
            domains[xi] = expression_reg.at(xi)->domains->clone(); 

        // File de propagation : variables dont le domaine a changé
        std::queue<int> changed;
        std::unordered_set<int> in_queue;

        // -- Init : appliquer revise sur chaque arc --
        for (const auto& [xi, neig_xi] : neighbors)
        {
            for (int xj : neig_xi)
            {
                auto& revise_fn = revise_reg.at(make_key(xi, xj));

                // revise_fn réduit domains[xi] selon domains[xj]
                IntervalPtr dom_xi = domains[xi];  // copie du ptr
                if (revise_fn(dom_xi, domains[xj]))
                {
                    domains[xi] = dom_xi;
                    if (dom_xi->flatten().empty()) return false;  // seul endroit où flatten est ok

                    if (!in_queue.count(xi))
                    {
                        changed.push(xi);
                        in_queue.insert(xi);
                    }
                }
            }
        }

        // -- Propagation --
        while (!changed.empty())
        {
            int xi = changed.front();
            changed.pop();
            in_queue.erase(xi);

            for (int xj : neighbors.at(xi))
            {
                auto& revise_fn = revise_reg.at(make_key(xj, xi));

                IntervalPtr dom_xj = domains[xj];
                if (revise_fn(dom_xj, domains[xi]))
                {
                    domains[xj] = dom_xj;
                    if (dom_xj->flatten().empty()) return false;

                    if (!in_queue.count(xj))
                    {
                        changed.push(xj);
                        in_queue.insert(xj);
                    }
                }
            }
        }

        // -- Répercuter --
        for (auto& [xi, dom] : domains)
        {
            auto& expr = expression_reg.at(xi);
            expr->domains = dom;
            expr->propagate_domains(dom);
        }

        return true;
    }
} // namespace kaiser::csp::core::arc


#endif
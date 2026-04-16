#ifndef __KAISER_CORE_CONSTRAINT_CONSTRAINT_BASE_HPP__
#define __KAISER_CORE_CONSTRAINT_CONSTRAINT_BASE_HPP__

#include "../expressions/ExpressionBase.hpp"
#include "../registry/Registry.hpp"
#include "../registry/NeighborsRegistry.hpp"
#include "../registry/ContextGuard.hpp"
#include <vector>
#include <span>
#include <memory>
#include <string>
#include <functional>

using namespace kaiser::csp::core::expression;
using namespace kaiser::csp::core::registry;

namespace kaiser::csp::core::constraint
{
    struct ConstraintBase;
    using Constraint = std::shared_ptr<ConstraintBase>;
    using FuncConstraint = std::function<bool (int, int)>;
    using ConstraintKey = uint64_t;
    using ConstraintRegistry = Registry<ConstraintKey, FuncConstraint>;
    using NeighborsRegistry = Registry<int, std::vector<int>>;

    using ReviseFunc = std::function<bool (IntervalPtr&, const IntervalPtr&)>;
    using ReviseRegistry = Registry<uint64_t, ReviseFunc>;

    struct ConstraintBase
    {
        int left_id, right_id;
        std::string context {"default"};

        ConstraintBase(std::string ctx, int l, int r)
        : context(std::move(ctx)), left_id(l), right_id(r)
        {
            auto& neig_reg = NeighborsRegistry::instance();
            ContextGuard neig_guard(neig_reg, context);
            neig_reg.add(l, r);
            neig_reg.add(r, l);
        }

        virtual void prepare() = 0;
        virtual bool compare(std::span<const int>) = 0;
        virtual ~ConstraintBase() = default;
    };
    
} // namespace kaiser::csp::core::constraint


#endif
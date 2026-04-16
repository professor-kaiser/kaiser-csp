#include "NotEqualConstraint.hpp"
#include "../structure/operation.hpp"
#include "../expressions/operation.hpp"
#include <iostream>

namespace kaiser::csp::core::constraint
{
    NotEqualConstraint::NotEqualConstraint(std::string ctx, int l, int r)
    : ConstraintBase(std::move(ctx), l, r)
    {
        auto& revise_reg = ReviseRegistry::instance();
        ContextGuard revise_guard(revise_reg, context);

        auto revise_fn = [] (IntervalPtr& x, const IntervalPtr& y)
        {
            const int y_dist = y->max() - y->min();
            if (y_dist != 0) return false;

            x -= y->max();
            return true;
        };

        revise_reg.register_type(make_key(l, r), revise_fn);
        revise_reg.register_type(make_key(r, l), std::move(revise_fn));
    }

    void NotEqualConstraint::prepare()
    {
        auto& flat_reg = FlattenRegistry::instance();
        ContextGuard guard(flat_reg, context);

        const auto& left_flat  = flat_reg.at(left_id);
        const auto& right_flat = flat_reg.at(right_id);
        cached_diff = diff(left_flat, right_flat);
    }

    bool NotEqualConstraint::compare(std::span<const int> data)
    {
        return eval(cached_diff, data) != 0;
    }
}
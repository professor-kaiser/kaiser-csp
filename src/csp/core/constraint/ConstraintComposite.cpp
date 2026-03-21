#include "ConstraintComposite.hpp"
#include <algorithm>

namespace kaiser::csp::core::constraint
{
    ConstraintComposite::ConstraintComposite(Relation relation)
    : relation_(relation), Constraint(nullptr)
    {
    }

    std::shared_ptr<ConstraintComposite> ConstraintComposite::add(const std::shared_ptr<Constraint>& constraint)
    {
        composites_.emplace_back(constraint);
        return shared_from_this();
    }

    std::shared_ptr<ConstraintComposite> ConstraintComposite::add(std::shared_ptr<Constraint>&& constraint)
    {
        composites_.emplace_back(constraint);
        return shared_from_this();
    }

    bool ConstraintComposite::compute(const std::vector<int>& data)
    {
        if (relation_ == Relation::AND)
        {
            return std::all_of(composites_.begin(), composites_.end(),
                [&](const auto& c) { return c->compute(data); });
        }
        else
        {
            return std::any_of(composites_.begin(), composites_.end(), 
                [&](const auto& c) { return c->compute(data); });
        }

        // int bit = (relation_ == Relation::OR) ? 0 : 1;

        // if (relation_ == Relation::AND) {
        //     for (const auto& c : composites_) {
        //         bit &= c->compute(data);
        //         if (!bit) return false;  // short-circuit
        //     }
        // } else {
        //     for (const auto& c : composites_) {
        //         bit |= c->compute(data);
        //         if (bit) return true;   // short-circuit
        //     }
        // }

        // return static_cast<bool>(bit);
    }
}
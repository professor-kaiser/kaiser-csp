#ifndef __KAISER_CORE_CONSTRAINT_COMPOSITE_HPP__
#define __KAISER_CORE_CONSTRAINT_COMPOSITE_HPP__

#include <vector>
#include <memory>
#include "Constraint.hpp"

namespace kaiser::csp::core::constraint
{
    class ConstraintComposite 
    : public Constraint, public std::enable_shared_from_this<ConstraintComposite>
    {
    protected:
        using Constraint::transformer_;
        std::vector<std::shared_ptr<Constraint>> composites_;

    public:
        enum class Relation { AND, OR } relation_;

        ConstraintComposite(Relation relation = Relation::AND);

        std::shared_ptr<ConstraintComposite> add(const std::shared_ptr<Constraint>& constraint);
        std::shared_ptr<ConstraintComposite> add(std::shared_ptr<Constraint>&& constraint);
        bool compute(const std::vector<int>&) override;
    };
}

#endif
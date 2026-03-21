#ifndef __KAISER_CORE_DISTINCT_CONSTRAINT_HPP__
#define __KAISER_CORE_DISTINCT_CONSTRAINT_HPP__

#include <vector>
#include "Constraint.hpp"

namespace kaiser::csp::core::constraint
{
    class DistinctConstraint final : public Constraint
    {
    private:
        using Constraint::transformer_;
        std::vector<int> indices_;

    public:
        DistinctConstraint(std::vector<int> indices);

        bool compute(const std::vector<int>&) override;
    };
}

#endif
#ifndef __KAISER_CORE_ACCUMULATE_CONSTRAINT_HPP__
#define __KAISER_CORE_ACCUMULATE__CONSTRAINT_HPP__

#include <vector>
#include <map>
#include <functional>
#include "Constraint.hpp"

namespace kaiser::csp::core::constraint
{
    class AccumulateConstraint : public Constraint
    {
    private:
        using Constraint::transformer_;
        std::map<int, std::vector<int>> rel_indices_;
        std::function<int (int, int)> accumulator_;
        int init_;

    public:
        AccumulateConstraint(
            std::map<int, std::vector<int>> rel_indices,
            std::function<int (int, int)> accumulator,
            int init = 0,
            std::function<std::vector<int> (const std::vector<int>&)> transformer = nullptr
        );

        bool compute(const std::vector<int>&) override;
    };
    
}

#endif
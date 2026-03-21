#include "MappedConstraint.hpp"
#include <algorithm>

namespace kaiser::csp::core::constraint
{
    MappedConstraint::MappedConstraint(
        std::map<int, std::vector<int>> rel_indices, 
        Operation operation)
    : rel_indices_(std::move(rel_indices)), operation_(operation), Constraint(nullptr)
    {
    }

    bool MappedConstraint::compute(const std::vector<int>& data)
    {
        auto func_comparer = compare[operation_];

        for (const auto& [idx, rel] : rel_indices_)
        {
            if (idx >= (int)data.size()) continue;

            if (!std::all_of(rel.begin(), rel.end(), [&](int ri) { 
                if (ri >= (int)data.size()) return true;
                return func_comparer(data[idx], data[ri]);
            }))
            {
                return false;
            }
        }
        
        return true;
    }
}
#include "MappedValueConstraint.hpp"
#include <algorithm>

namespace kaiser::csp::core::constraint
{
    MappedValueConstraint::MappedValueConstraint(
        std::map<int, std::vector<int>> rel_indices, 
        Operation operation)
    : rel_indices_(std::move(rel_indices)), operation_(operation), Constraint(nullptr)
    {
    }

    bool MappedValueConstraint::compute(const std::vector<int>& data)
    {
        auto& func_comparer = compare[operation_];

        for (const auto& [idx, rel] : rel_indices_)
        {
            if (idx >= (int)data.size()) continue;

            if (!std::all_of(rel.begin(), rel.end(), [&](int x) {
                return func_comparer(data[idx], x);
            }))
            {
                return false;
            }
        }
        
        return true;
    }
}
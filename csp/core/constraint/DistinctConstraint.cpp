#include "DistinctConstraint.hpp"
#include <algorithm>
#include <cstdint>

namespace kaiser::csp::core::constraint
{
    DistinctConstraint::DistinctConstraint(std::vector<int> indices)
    : Constraint(nullptr)
    {
        std::sort(indices.begin(), indices.end());
        indices_ = std::move(indices);
    }

    bool DistinctConstraint::compute(const std::vector<int>& data)
    {
        uint64_t seen = 0;

        for (size_t i = 0; i < indices_.size(); i++)
        {
            int index = indices_[i];

            if (index >= (int)data.size()) break;

            uint64_t bit = uint64_t(1) << data[index];

            if (seen & bit)
                return false;

            seen |= bit;
        }

        return true;
    }
}
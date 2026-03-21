#include "AccumulateConstraint.hpp"
#include <algorithm>
#include <numeric>

namespace kaiser::csp::core::constraint
{
    AccumulateConstraint::AccumulateConstraint(
        std::map<int, std::vector<int>> rel_indices,
        std::function<int (int, int)> accumulator,
        int init,
        std::function<std::vector<int> (const std::vector<int>&)> transformer
    )
    : rel_indices_(rel_indices), 
      accumulator_(accumulator), 
      init_(init),
      Constraint(transformer)
    {}

    bool AccumulateConstraint::compute(const std::vector<int>& data)
    {
        std::vector<int> transformed_data = transformer_(data);
        if (transformed_data.empty())
            return true;

        for (const auto& [idx, rel] : rel_indices_)
        {
            if (idx >= (int)transformed_data.size()) continue;

            int acc = std::accumulate(
                rel.begin(), rel.end(), init_,
                [&](int acc, int i) { return accumulator_(acc, transformed_data[i]); }
            );

            if (acc != transformed_data[idx])
                return false;
        }

        // std::vector<int> subdata;

        // for (const auto& [idx, rel] : rel_indices_)
        // {
        //     if (idx >= (int)transformed_data.size()) continue;
        //     subdata.clear();
        //     subdata.resize(rel.size());

        //     //
        //     // std::transform(rel.begin(), rel.end(), 
        //     //     std::back_inserter(subdata), [&](int i) { return transformed_data[i]; });
        //     //
        //     for (size_t i = 0; i < rel.size(); i++)
        //         subdata[i] = transformed_data[rel[i]];

        //     int sum = std::accumulate(
        //         subdata.begin(), 
        //         subdata.end(), 
        //         init_, accumulator_);

        //     if (sum != transformed_data[idx])
        //         return false;
        // }
        
        return true;
    }
}

#ifndef __KAISER_CORE_MAPPED_CONSTRAINT_HPP__
#define __KAISER_CORE_MAPPED_CONSTRAINT_HPP__

#include "Constraint.hpp"
#include <map>
#include <functional>

namespace kaiser::csp::core::constraint
{
    class MappedConstraint : public Constraint
    {
    public:
        enum class Operation
        {
            EQ, NEQ,
            GT, LT,
            GTEQ, LTEQ
        };
        
        MappedConstraint(
            std::map<int, std::vector<int>> rel_indices, 
            Operation operation = Operation::EQ);

        bool compute(const std::vector<int>&) override;

    private:
        using Constraint::transformer_;
        std::map<int, std::vector<int>> rel_indices_;
        Operation operation_;

        std::map<MappedConstraint::Operation, std::function<bool (int, int)>> compare = {
            { Operation::EQ,   [](int a, int b) { return a == b; } },
            { Operation::NEQ,  [](int a, int b) { return a != b; } },
            { Operation::GT,   [](int a, int b) { return a > b;  } },
            { Operation::LT,   [](int a, int b) { return a < b;  } },
            { Operation::GTEQ, [](int a, int b) { return a >= b; } },
            { Operation::LTEQ, [](int a, int b) { return a <= b; } },
        };

    };
}

#endif
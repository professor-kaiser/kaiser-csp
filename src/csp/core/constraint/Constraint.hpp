#ifndef __KAISER_CORE_CONSTRAINT_HPP__
#define __KAISER_CORE_CONSTRAINT_HPP__

#include <vector>
#include <functional>

namespace kaiser::csp::core::constraint
{
    class Constraint
    {
    protected:
        std::function<std::vector<int> (const std::vector<int>&)> transformer_;

    public:
        Constraint(std::function<std::vector<int> (const std::vector<int>&)> transformer)
        : transformer_(transformer)
        {}
        
        virtual bool compute(const std::vector<int>&) = 0;
        virtual ~Constraint() = default;
    };
}

#endif
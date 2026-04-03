#include "EqualConstraint.hpp"

namespace kaiser::csp::core::constraint
{
    EqualConstraint::EqualConstraint(Flat l, Flat r)
    : ConstraintBase(l, r) {}

    bool EqualConstraint::compare(const std::vector<int>& data)
    {
        const int l = eval(left, data);
        const int r = eval(left, data);

        return l == r;
    }

    Constraint operator==(Flat left, Flat right)
    {
        return std::make_shared<EqualConstraint>(left, right);
    }
}
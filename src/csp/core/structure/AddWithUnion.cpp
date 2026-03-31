#include "AddWithUnion.hpp"
#include <vector>

namespace kaiser::csp::core::structure
{
    AddWithUnion::AddWithUnion(const UnionInterval& l)
    : left(l) {}

    IntervalPtr AddWithUnion::visit(const ContinuousInterval& right) const
    {
        UnionInterval right_as_union({right});
        return std::make_shared<UnionInterval>(left + right_as_union);
    }

    IntervalPtr AddWithUnion::visit(const UnionInterval& right) const
    {
        return std::make_shared<UnionInterval>(left + right);
    }
}
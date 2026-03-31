#include "AddWithContinuous.hpp"
#include <vector>

namespace kaiser::csp::core::structure
{
    AddWithContinuous::AddWithContinuous(const ContinuousInterval& l)
    : left(l) {}

    IntervalPtr AddWithContinuous::visit(const ContinuousInterval& right) const
    {
        return std::make_shared<ContinuousInterval>(left + right);
    }

    IntervalPtr AddWithContinuous::visit(const UnionInterval& right) const
    {
        UnionInterval left_as_union({left});
        return std::make_shared<UnionInterval>(left_as_union + right);
    }
}
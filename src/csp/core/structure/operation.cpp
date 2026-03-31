#include "operation.hpp"

namespace kaiser::csp::core::structure
{
    IntervalPtr operator+(const IntervalPtr& left, const IntervalPtr& right)
    {
        struct LeftDispatch : IntervalAddVisitor 
        {
            const IntervalPtr& right;
            explicit LeftDispatch(const IntervalPtr& r) : right(r) {}

            IntervalPtr visit(const ContinuousInterval& l) const override 
            {
                return right->accept(AddWithContinuous{l});
            }

            IntervalPtr visit(const UnionInterval& l) const override 
            {
                return right->accept(AddWithUnion{l});
            }
        };

        return left->accept(LeftDispatch{right});
    }
    
    IntervalPtr operator*(IntervalPtr interval, double k)
    {
        return (*interval) * k;
    }

    IntervalPtr operator*=(IntervalPtr interval, double k)
    {
        (*interval) *= k;
        return interval;
    }
} // namespace kaiser::csp::core::structure
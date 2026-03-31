#ifndef __KAISER_CORE_EXPRESSIONS_EXPRESSIONBASE_HPP__
#define __KAISER_CORE_EXPRESSIONS_EXPRESSIONBASE_HPP__

#include <map>
#include <array>
#include <memory>
#include <cassert>
#include <vector>
#include "../structure/Interval.hpp"

using namespace kaiser::csp::core::structure;

namespace kaiser::csp::core::expression
{
    struct Flat
    {
        std::map<int, int> coeffs;
        int constant;
    };

    struct ExpressionBase
    {
        IntervalPtr domains;

        ExpressionBase(IntervalPtr bounds)
        : domains(std::move(bounds))
        {
        }

        virtual void propagate_domains(IntervalPtr) = 0;
        virtual Flat flatten() const = 0;
        virtual ~ExpressionBase() = default;
    };

    template <std::size_t _N>
    int eval(const Flat& flat, const std::array<int, _N>& vals)
    {
        int result = flat.constant;
        const auto& coeffs = flat.coeffs;

        for (const auto& [idx, coeff] : coeffs)
        {
            result += coeff * vals[idx];
        }

        return result;
    }
    
} // namespace kaiser::csp::core::expression


#endif
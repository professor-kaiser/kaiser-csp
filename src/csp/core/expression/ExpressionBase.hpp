#ifndef __KAISER_CORE_EXPRESSION_EXPRESSIONBASE_HPP__
#define __KAISER_CORE_EXPRESSION_EXPRESSIONBASE_HPP__

#include <map>
#include <array>
#include <memory>
#include <cassert>
#include "structure.hpp"

namespace kaiser::csp::core::expression
{
    using namespace structure;

    template <typename T>
    struct ExpressionBase
    {
        using value_type = T;
        Bounds<T> domains;
        //std::vector<T> domains;

        ExpressionBase(Bounds<T> bounds)
        : domains(std::move(bounds))
        {
        }

        virtual void propagate_domains(Bounds<T>) = 0;
        virtual Flat<T> flatten() const = 0;
        virtual ~ExpressionBase() = default;
    };

    template <typename T, std::size_t _N>
    T eval(const std::vector<std::pair<int, T>>& terms, T constant, const std::array<T, _N>& vals)
    {
        T result = constant;
        for (const auto& [idx, coeff] : terms)
        {
            result += coeff * vals[idx];
        }

        return result;
    }

    template <typename T, std::size_t _N>
    T eval(const Flat<T>& flat, const std::array<T, _N>& vals)
    {
        T result = flat.constant;
        const auto& coeffs = flat.coeffs;

        for (const auto& [idx, coeff] : coeffs)
        {
            result += coeff * vals[idx];
        }

        return result;
    }

    template <typename T, std::size_t _N>
    T eval(const std::shared_ptr<ExpressionBase<T>>& expr, const std::array<T, _N>& vals)
    {
        auto [coeffs, constant] = expr->flatten();
        T result = constant;

        for (const auto& [idx, coeff] : coeffs)
        {
            result += coeff * vals[idx];
        }

        return result;
    }

    template <typename T>
    T eval(const std::shared_ptr<ExpressionBase<T>>& expr, const std::vector<T>& vals)
    {
        auto [coeffs, constant] = expr->flatten();
        T result = constant;

        for (const auto& [idx, coeff] : coeffs)
        {
            result += coeff * vals[idx];
        }

        return result;
    }
    
} // namespace kaiser::csp::core::expression


#endif
#ifndef __KAISER_CORE_EXPRESSIONS_EXPRESSIONBASE_HPP__
#define __KAISER_CORE_EXPRESSIONS_EXPRESSIONBASE_HPP__

#include <map>
#include <array>
#include <memory>
#include <cassert>
#include <vector>
#include "../structure/Interval.hpp"
#include "../registry/RegistryBase.hpp"

using namespace kaiser::csp::core::structure;
using namespace kaiser::csp::core::registry;

namespace kaiser::csp::core::expression
{
    struct Flat
    {
        std::map<int, int> coeffs;
        int constant;
    };

    struct ExpressionBase;
    using Expression = std::shared_ptr<ExpressionBase>;

    using ExpressionRegistry = RegistryBase<int, Expression>;

    struct ExpressionBase : std::enable_shared_from_this<ExpressionBase>
    {
    private: 
        inline static int count = 0;

    public:
        IntervalPtr domains;
        int id;

        ExpressionBase(IntervalPtr bounds)
        : domains(std::move(bounds))
        {
        }

        virtual void init();
        virtual void propagate_domains(IntervalPtr) = 0;
        virtual Flat flatten() const = 0;
        virtual ~ExpressionBase() = default;
    };

    inline void ExpressionBase::init()
    {
        id = count++;
        ExpressionRegistry::instance()
            .register_type(id, shared_from_this());
    }

    template <typename T, typename... Args>
    std::shared_ptr<T> make_expression(Args&&... args)
    {
        auto ptr = std::make_shared<T>(std::forward<Args>(args)...);
        ptr->init();

        return ptr;
    }

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

    inline int eval(const Flat& flat, const std::vector<int>& vals)
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
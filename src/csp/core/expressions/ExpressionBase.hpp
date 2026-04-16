#ifndef __KAISER_CORE_EXPRESSIONS_EXPRESSIONBASE_HPP__
#define __KAISER_CORE_EXPRESSIONS_EXPRESSIONBASE_HPP__

#include <array>
#include <memory>
#include <cassert>
#include <vector>
#include <string>
#include "Flat.hpp"
#include "../structure/Interval.hpp"
#include "../registry/Registry.hpp"
#include "../registry/ContextGuard.hpp"

using namespace kaiser::csp::core::structure;
using namespace kaiser::csp::core::registry;

namespace kaiser::csp::core::expression
{
    struct ExpressionBase;
    using Expression = std::shared_ptr<ExpressionBase>;

    using ExpressionRegistry = Registry<int, Expression>;
    using FlattenRegistry = Registry<int, Flat>;
    using FlatDomainsRegistry = Registry<int, std::set<int>>;
    using DomainsRegistry = Registry<int, IntervalPtr>;

    struct ExpressionBase : std::enable_shared_from_this<ExpressionBase>
    {
    protected: 
        inline static int count = 0;
        std::string context_ {"default"};

    public:
        IntervalPtr domains;
        int id;

        ExpressionBase(std::string context, IntervalPtr bounds)
        : context_(std::move(context)), domains(std::move(bounds))
        {
        }

        ExpressionBase(IntervalPtr bounds)
        : domains(std::move(bounds))
        {
        }

        std::string get_context() const { return context_; }
        void set_context(std::string context) { context_ = std::move(context); }
        virtual void init();
        void update_domains();
        virtual void propagate_domains(IntervalPtr) = 0;
        virtual Flat flatten() const = 0;
        virtual ~ExpressionBase() = default;
    };

    inline void ExpressionBase::init()
    {
        id = count++;

        auto& expr_reg = ExpressionRegistry::instance();
        ContextGuard expr_guard(expr_reg, context_);
        expr_reg.register_type(id, shared_from_this());

        auto& flat_reg = FlattenRegistry::instance();
        ContextGuard flat_guard(flat_reg, context_);
        flat_reg.register_type(id, flatten());

        update_domains();
    }

    inline void ExpressionBase::update_domains()
    {
        // Flattened
        auto& fdoms_reg = FlatDomainsRegistry::instance();
        ContextGuard fdoms_guard(fdoms_reg, context_);
        fdoms_reg.register_type(id, domains->flatten());

        // // Class
        // auto& doms_reg = DomainsRegistry::instance();
        // ContextGuard doms_guard(doms_reg, context_);
        // doms_reg.register_type(id, domains->flatten());
    }

    template <typename T, typename... Args>
    std::shared_ptr<T> make_expression(Args&&... args)
    {
        auto ptr = std::make_shared<T>(std::forward<Args>(args)...);
        ptr->init();

        return ptr;
    }

    template <typename TRange>
    int eval(const Flat& flat, const TRange& vals)
    {
        int result = flat.constant;
        for (const auto& [idx, coeff] : flat.coeffs)
            result += coeff * vals[idx];
        return result;
    }
    
} // namespace kaiser::csp::core::expression

#endif
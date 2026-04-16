#ifndef __KAISER_CORE_REGISTRY_REGISTRY_GUARD_HPP__
#define __KAISER_CORE_REGISTRY_REGISTRY_GUARD_HPP__

#include "Registry.hpp"
#include "ContextGuardBase.hpp"
#include <string>
#include <memory>

namespace kaiser::csp::core::registry
{
    template <typename TId, typename TRegistry>
    class ContextGuard : public ContextGuardBase
    {
    private:
        Registry<TId, TRegistry>& reg_;
        std::string previous_;

    public:
        ContextGuard(Registry<TId, TRegistry>& r, const std::string& ctx);
        ~ContextGuard();
    };

    template <typename TId, typename TRegistry>
    std::unique_ptr<ContextGuardBase> make_context_guard(
        Registry<TId, TRegistry>& r, const std::string& ctx);
}

#include "ContextGuard.tpp"

#endif

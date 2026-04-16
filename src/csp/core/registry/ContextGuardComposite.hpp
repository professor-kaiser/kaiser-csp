#ifndef __KAISER_CORE_REGISTRY_REGISTRY_GUARD_COMPOSITE_HPP__
#define __KAISER_CORE_REGISTRY_REGISTRY_GUARD_COMPOSITE_HPP__

#include "ContextGuardBase.hpp"
#include <memory>
#include <vector>

namespace kaiser::csp::core::registry
{
    class ContextGuardComposite 
    : public ContextGuardBase, public std::enable_shared_from_this<ContextGuardComposite>
    {
    private:
        std::vector<std::unique_ptr<ContextGuardBase>> guards_;

    public:
        ContextGuardComposite() = default;
        ContextGuardComposite& add(std::unique_ptr<ContextGuardBase>);
    };
}

#include "ContextGuardComposite.tpp"

#endif

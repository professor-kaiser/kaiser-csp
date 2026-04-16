#ifndef __KAISER_CORE_REGISTRY_CONTEXT_GUARD_BASE_HPP__
#define __KAISER_CORE_REGISTRY_CONTEXT_GUARD_BASE_HPP__

#include <string>

namespace kaiser::csp::core::registry
{
    struct ContextGuardBase
    {
        virtual ~ContextGuardBase() = default;
    };
}

#endif

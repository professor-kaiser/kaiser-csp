#ifndef __KAISER_CORE_REGISTRY_REGISTRYBASE_HPP__
#define __KAISER_CORE_REGISTRY_REGISTRYBASE_HPP__

#include <unordered_map>

namespace kaiser::csp::core::registry
{
    template <typename TId, typename TRegistry>
    class RegistryBase
    {
    private:
        std::unordered_map<TId, TRegistry> registry;
        RegistryBase() = default;
        
    public:
        RegistryBase(const RegistryBase&) = delete;
        RegistryBase& operator=(const RegistryBase&) = delete;

        static RegistryBase& instance()
        {
            static RegistryBase inst;
            return inst;
        }

        template <typename U>
        void register_type(TId id, U&& reg)
        {
            registry[id] = std::forward<U>(reg);
        }

        TRegistry& at(TId id)
        {
            return registry.at(id);
        }

        bool contains(TId id) const
        {
            return registry.find(id) != registry.end();
        }
    };
    
} // namespace kaiser::csp::core::registry

#endif
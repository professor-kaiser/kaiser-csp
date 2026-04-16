#ifndef __KAISER_CORE_REGISTRY_REGISTRYBASE_HPP__
#define __KAISER_CORE_REGISTRY_REGISTRYBASE_HPP__

#include <unordered_map>
#include <string>
#include <cstdint>

namespace kaiser::csp::core::registry
{
    inline uint64_t make_key(int a, int b)
    {
        return (static_cast<uint64_t>(a) << 32) | b;
    }

    template <typename TId, typename TRegistry>
    class Registry
    {
    private:
        std::unordered_map<std::string, std::unordered_map<TId, TRegistry>> registry;
        std::string current_context_ = "default";
        Registry() = default;
        
    public:
        Registry(const Registry&) = delete;
        Registry& operator=(const Registry&) = delete;

        static Registry& instance();
        void set_context(const std::string& context);
        const std::string& current_context() const;
        void clear_context(const std::string& context);
        template <typename U> void register_type(TId id, U&& reg);
        TRegistry& at(TId id);
        std::unordered_map<TId, TRegistry>& get_registry();
        std::unordered_map<TId, TRegistry> get_registry() const;
        bool contains(TId id) const;
    };
    
} // namespace kaiser::csp::core::registry


#include "Registry.tpp"

#endif
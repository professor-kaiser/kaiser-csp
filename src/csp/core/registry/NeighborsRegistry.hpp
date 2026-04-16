#ifndef __KAISER_CORE_REGISTRY_REGISTRY_VECTOR_HPP__
#define __KAISER_CORE_REGISTRY_REGISTRY_VECTOR_HPP__

#include "Registry.hpp"
#include <vector>

namespace kaiser::csp::core::registry
{
    template <>
    class Registry<int, std::vector<int>>
    {
    private:
        std::unordered_map<std::string, std::unordered_map<int, std::vector<int>>> registry;
        std::string current_context_ = "default";
        Registry() = default;
        
    public:
        Registry(const Registry&) = delete;
        Registry& operator=(const Registry&) = delete;

        static Registry& instance()
        {
            static Registry inst;
            return inst;
        }
        

        void set_context(const std::string& context)
        {
            current_context_ = context;
        }

        const std::string& current_context() const
        {
            return current_context_;
        }

        void clear_context(const std::string& context)
        {
            registry.erase(context);
        }

        template <typename U>
        void register_type(int id, U&& reg)
        {
            registry[current_context_][id] = std::forward<U>(reg);
        }

        void add(int id, int element)
        {
            if (!contains(id)) registry[current_context_][id] = {};
            registry[current_context_][id].push_back(element);
        }

        void add(int id, const std::vector<int>& v)
        {
            if (!contains(id)) registry[current_context_][id] = {};
            auto& vec = registry[current_context_][id];
            vec.insert(vec.end(), v.begin(), v.end());
        }

        std::vector<int>& at(int id)
        {
            auto it = registry.find(current_context_);
            if (it == registry.end()) 
                throw std::runtime_error("context not found: " + current_context_);
            auto it2 = it->second.find(id);
            if (it2 == it->second.end()) 
                throw std::runtime_error("id not found: " + std::to_string(id) + " in context: " + current_context_);
            return it2->second;
        }

        std::unordered_map<int, std::vector<int>> get_registry() const
        {
            auto it = registry.find(current_context_);
            if (it == registry.end()) return {};
            return it->second;
        }

        bool contains(int id) const
        {
            auto it = registry.find(current_context_);
            if (it == registry.end()) return false;
            return it->second.find(id) != it->second.end();
        }
    };
    
} // namespace kaiser::csp::core::registry

#endif
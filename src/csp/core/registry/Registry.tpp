namespace kaiser::csp::core::registry
{
    template <typename TId, typename TRegistry>
    Registry<TId, TRegistry>& Registry<TId, TRegistry>::instance()
    {
        static Registry inst;
        return inst;
    }

    template <typename TId, typename TRegistry>
    void Registry<TId, TRegistry>::set_context(const std::string& context)
    {
        current_context_ = context;
    }

    template <typename TId, typename TRegistry>
    const std::string& Registry<TId, TRegistry>::current_context() const
    {
        return current_context_;
    }

    template <typename TId, typename TRegistry>
    void Registry<TId, TRegistry>::clear_context(const std::string& context)
    {
        registry.erase(context);
    }

    template <typename TId, typename TRegistry>
    template <typename U>
    void Registry<TId, TRegistry>::register_type(TId id, U&& reg)
    {
        registry[current_context_][id] = std::forward<U>(reg);
    }

    template <typename TId, typename TRegistry>
    TRegistry& Registry<TId, TRegistry>::at(TId id)
    {
        auto it = registry.find(current_context_);
        if (it == registry.end()) 
            throw std::runtime_error("context not found: " + current_context_);
        auto it2 = it->second.find(id);
        if (it2 == it->second.end()) 
            throw std::runtime_error("id not found: " + std::to_string(id) + " in context: " + current_context_);
        return it2->second;
    }

    template <typename TId, typename TRegistry>
    std::unordered_map<TId, TRegistry>& Registry<TId, TRegistry>::get_registry()
    {
        auto it = registry.find(current_context_);
        if (it == registry.end())
        {
            registry[current_context_] = {};
            return registry[current_context_];
        }
        return it->second;
    }

    template <typename TId, typename TRegistry>
    std::unordered_map<TId, TRegistry> Registry<TId, TRegistry>::get_registry() const
    {
        auto it = registry.find(current_context_);
        if (it == registry.end()) return {};
        return it->second;
    }

    template <typename TId, typename TRegistry>
    bool Registry<TId, TRegistry>::contains(TId id) const
    {
        auto it = registry.find(current_context_);
        if (it == registry.end()) return false;
        return it->second.find(id) != it->second.end();
    }
}
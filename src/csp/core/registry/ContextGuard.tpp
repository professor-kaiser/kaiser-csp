namespace kaiser::csp::core::registry
{
    template <typename TId, typename TRegistry>
    ContextGuard<TId, TRegistry>::ContextGuard(Registry<TId, TRegistry>& r, const std::string& ctx)
    : reg_(r), previous_(r.current_context())
    {
        reg_.set_context(ctx);
    }

    template <typename TId, typename TRegistry>
    ContextGuard<TId, TRegistry>::~ContextGuard() 
    { 
        reg_.set_context(previous_);
    }

    template <typename TId, typename TRegistry>
    std::unique_ptr<ContextGuardBase> make_context_guard(
        Registry<TId, TRegistry>& r, const std::string& ctx)
    {
        return std::make_unique<ContextGuard<TId, TRegistry>>(r, ctx);
    }
}
namespace kaiser::csp::core::registry
{
    ContextGuardComposite& ContextGuardComposite::add(
        std::unique_ptr<ContextGuardBase> guard)
    {
        guards_.push_back(std::move(guard));
        return *this;
    }
}
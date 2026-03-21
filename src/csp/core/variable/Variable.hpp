#ifndef __KAISER_CORE_VARIABLE_HPP__
#define __KAISER_CORE_VARIABLE_HPP__

#include <vector>
#include <memory>

namespace kaiser::csp::core::variable
{
    class Variable : public std::enable_shared_from_this<Variable>
    {
    private:
        std::vector<int> domains_;
        std::vector<size_t> offsets_;
        size_t size_;

    public:
        Variable() = default;
        std::shared_ptr<Variable> add(std::vector<int> domains);
        std::vector<int> get_domain(int idx);
        size_t domains_total_size() const;
        size_t size() const;
    };
}

#endif
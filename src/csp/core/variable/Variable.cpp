#include "Variable.hpp"
#include <cstring>

namespace kaiser::csp::core::variable
{
    size_t Variable::size() const
    {
        return offsets_.size();
    }

    size_t Variable::domains_total_size() const
    {
        return domains_.size();
    }

    std::vector<int> Variable::get_domain(int idx)
    {
        if (domains_.empty() || idx < 0) return {};

        size_t offset = offsets_[idx];
        size_t prev_offset = idx == 0 ? 0 : offsets_[idx - 1];
        size_t domain_size = offset - prev_offset;

        std::vector<int> domain(domain_size);
        std::memcpy(
            domain.data(), 
            domains_.data() + prev_offset, 
            domain_size * sizeof(int));

        return domain;
    }

    std::shared_ptr<Variable> Variable::add(std::vector<int> domains)
    {
        size_t prev_offset = offsets_.empty() ? 0 : offsets_.back();
        size_t new_offset  = prev_offset + domains.size();

        domains_.insert(domains_.end(), domains.begin(), domains.end());;
        offsets_.emplace_back(new_offset);

        return shared_from_this();
    }
}
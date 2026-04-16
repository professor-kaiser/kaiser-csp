#include <map>

namespace kaiser::csp::core::expression
{
    struct Flat
    {
        std::map<int, int> coeffs;
        int constant;
    };

    inline Flat diff(const Flat& left, const Flat& right)
    {
        Flat cache_diff = left;
        for (const auto& [idx, coeff] : right.coeffs)
            cache_diff.coeffs[idx] -= coeff;
        cache_diff.constant -= right.constant;

        return cache_diff;
    }
}
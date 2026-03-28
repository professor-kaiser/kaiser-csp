#ifndef __KAISER_CORE_ARC_HPP__
#define __KAISER_CORE_ARC_HPP__

#include "../expression/structure.hpp"
#include "../expression/ExpressionBase.hpp"
#include <functional>
#include <vector>
#include <algorithm>

using namespace kaiser::csp::core::expression;

namespace kaiser::csp::core::arc
{
    template <typename T>
    using BinaryFuncConstraint = std::function<bool (T, T)>;

    // template <typename T>
    // bool revise(
    //     std::vector<T>& x_vec, 
    //     const std::vector<T>& y_vec, 
    //     BinaryFuncConstraint<T> constraint)
    // {
    //     auto old_size = x_vec.size();

    //     auto it = std::remove_if(x_vec.begin(), x_vec.end(), [&](T x) {
    //         return !std::any_of(y_vec.begin(), y_vec.end(), [&](T y) {
    //             return constraint(x, y);
    //         });
    //     });

    //     x_vec.erase(it, x_vec.end());

    //     return x_vec.size() != old_size;
    // }

    template <typename T>
    bool revise(
        std::vector<T>& x_vec,
        const std::vector<T>& y_vec,
        const BinaryFuncConstraint<T>& constraint)
    {
        if (x_vec.empty() || y_vec.empty()) 
        {
            bool changed = !x_vec.empty();
            x_vec.clear();
            return changed;
        }

        const auto it = std::remove_if(x_vec.begin(), x_vec.end(), 
            [&](const T& x) {
                return !std::any_of(y_vec.begin(), y_vec.end(), 
                    [&](const T& y) { return constraint(x, y); });
            });

        if (it == x_vec.end()) return false;

        x_vec.erase(it, x_vec.end());
        return true;
    }

} // namespace kaiser::csp::core::arc


#endif
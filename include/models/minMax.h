// SPDX-License-Identifier: MIT

#pragma once

#include <type_traits>

#include "utils/approximate.h"
#include "utils/typeTraits.h"

namespace monsterCatVisualizer
{

template <typename T>
struct MinMax
{
    constexpr bool contains(T value, bool includingBounds) const noexcept
    {
        if constexpr (std::is_floating_point_v<T>)
        {
            return includingBounds
                ? value >= min && value <= max
                : utils::Approximate::contains(value, min, max);
        }
        else if constexpr (std::is_integral_v<T>)
        {
            return includingBounds
                ? value >= min && value <= max
                : value > min && value < max;
        }
        else
        {
            return false;
        }
    }

    T min{};
    T max{};
};

template <typename T, typename = std::enable_if_t<utils::is_equality_comparable_v<T>>>
constexpr bool operator==(const MinMax<T>& lhs, const MinMax<T>& rhs) noexcept
{
    return lhs.min == rhs.min && lhs.max == rhs.max;
}
template <typename T, typename = std::enable_if_t<utils::is_equality_comparable_v<T>>>
constexpr bool operator!=(const MinMax<T>& lhs, const MinMax<T>& rhs) noexcept
{
    return !(lhs == rhs);
}

} // namespace monsterCatVisualizer

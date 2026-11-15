// SPDX-License-Identifier: MIT

#pragma once

#include <algorithm>
#include <cstdint>
#include <limits>
#include <type_traits>

namespace monsterCatVisualizer
{

template <typename T = std::uint8_t>
struct ColorRGB
{
    static_assert(
        std::is_floating_point_v<T> ||
        (std::is_integral_v<T> && std::is_unsigned_v<T> && !std::is_same_v<T, bool>),
        "ColorRGB component type must be floating-point or unsigned integral");

    ColorRGB constexpr getNormalized() const
    {
        ColorRGB result{*this};
        result.normalize();
        return result;
    }

    [[maybe_unused]] constexpr ColorRGB& normalize()
    {
        if constexpr (std::is_floating_point_v<T>)
        {
            r = std::clamp(r, T{0}, T{1});
            g = std::clamp(g, T{0}, T{1});
            b = std::clamp(b, T{0}, T{1});
        }
        return *this;
    }

    T r{};
    T g{};
    T b{};
};

inline constexpr std::uint32_t packColor(ColorRGB<> color) noexcept
{
    return (static_cast<std::uint32_t>(color.r) << 16u) |
        (static_cast<std::uint32_t>(color.g) << 8u) |
        static_cast<std::uint32_t>(color.b);
}

inline constexpr ColorRGB<> unpackColor(std::uint32_t color) noexcept
{
    return ColorRGB<>
    {
        static_cast<std::uint8_t>((color >> 16u) & 0xffu),
        static_cast<std::uint8_t>((color >> 8u) & 0xffu),
        static_cast<std::uint8_t>(color & 0xffu)
    };
}


template <typename T = std::uint8_t, typename U = float>
struct ColorRGBA
{
    static_assert(
        std::is_floating_point_v<T> ||
        (std::is_integral_v<T> && std::is_unsigned_v<T> && !std::is_same_v<T, bool>),
        "ColorRGBA color component type must be floating-point or unsigned integral");
    static_assert(
        std::is_floating_point_v<U> ||
        (std::is_integral_v<U> && std::is_unsigned_v<U> && !std::is_same_v<U, bool>),
        "ColorRGBA opacity component type must be floating-point or unsigned integral");

    constexpr ColorRGBA getNormalized() const
    {
        ColorRGBA result{*this};
        result.normalize();
        return result;
    }

    [[maybe_unused]] constexpr ColorRGBA& normalize()
    {
        if constexpr (std::is_floating_point_v<T>)
        {
            r = std::clamp(r, T{0}, T{1});
            g = std::clamp(g, T{0}, T{1});
            b = std::clamp(b, T{0}, T{1});
        }
        if constexpr (std::is_floating_point_v<U>)
        {
            a = std::clamp(a, U{0}, U{1});
        }

        return *this;
    }

    T r{};
    T g{};
    T b{};
    U a{std::is_floating_point_v<U> ? U{1} : std::numeric_limits<U>::max()};
};

} // namespace monsterCatVisualizer

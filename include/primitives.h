// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>
#include <limits>
#include <type_traits>

namespace monsterCatVisualizer
{

template <typename T = float>
struct Vector2
{
    T x{};
    T y{};
};

template <typename T = std::uint8_t>
struct ColorRGB
{
    static_assert(
        std::is_floating_point_v<T> ||
        (std::is_integral_v<T> && std::is_unsigned_v<T> && !std::is_same_v<T, bool>),
        "ColorRGB component type must be floating-point or unsigned integral");

    T r{};
    T g{};
    T b{};
};

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

    T r{};
    T g{};
    T b{};
    U a{std::is_floating_point_v<U> ? U{1} : std::numeric_limits<U>::max()};
};

} // namespace monsterCatVisualizer

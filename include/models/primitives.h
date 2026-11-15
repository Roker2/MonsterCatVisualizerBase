// SPDX-License-Identifier: MIT

#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>
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

template <typename T = float>
struct Vector2
{
    // Arithmetic does not check for overflow or out-of-range conversions.
    // Callers must ensure results are representable; otherwise, UB is possible.

    static_assert(
        std::is_floating_point_v<T> ||
        (std::is_integral_v<T> && std::is_signed_v<T> && !std::is_same_v<T, bool>),
        "Vector2 component type must be floating-point or signed integral");

    using LengthType = std::common_type_t<T, float>;

    LengthType getSquaredLength() const
    {
        const LengthType wideX = static_cast<LengthType>(x);
        const LengthType wideY = static_cast<LengthType>(y);
        return wideX * wideX + wideY * wideY;
    }

    LengthType getLength() const
    {
        return std::sqrt(getSquaredLength());
    }

    Vector2 getNormalized() const
    {
        Vector2 result{*this};
        result.normalize();
        return result;
    }

    Vector2& normalize()
    {
        static_assert(std::is_floating_point_v<T>,
            "Normalizing an integral vector would truncate its components");

        const LengthType length = getLength();
        if (utils::Approximate::lessEqualZero(length) || !std::isfinite(length))
        {
            x = T{0};
            y = T{0};
        }
        else
        {
            x = static_cast<T>(x / length);
            y = static_cast<T>(y / length);
        }

        return *this;
    }

    template <typename U>
    Vector2 operator*(U scale) const
    {
        return Vector2
        {
            static_cast<T>(x * scale),
            static_cast<T>(y * scale)
        };
    }

    template <typename U>
    Vector2 operator+(const Vector2<U>& other) const
    {
        return Vector2
        {
            static_cast<T>(x + other.x),
            static_cast<T>(y + other.y)
        };
    }

    template <typename U>
    Vector2 operator-(const Vector2<U>& other) const
    {
        return Vector2
        {
            static_cast<T>(x - other.x),
            static_cast<T>(y - other.y)
        };
    }

    Vector2 operator-() const
    {
        return Vector2{-x, -y};
    }

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

struct TrajectoryDescriptor
{
    /** Calculate current position in NVS */
    Vector2<> calculatePosition(double timeSeconds,
        float viewportWidth,
        float viewportHeight) const;
    /** Calculate current position in NVS */
    Vector2<> calculatePosition(double timeSeconds,
        float viewportRatio) const;

    /** Start position in NVS */
    Vector2<> startPosition{};
    /** Movement direction in NSS */
    Vector2<> direction{};
    /** Speed in NVS */
    float speed{};
    double startTimeSeconds{};
};

} // namespace monsterCatVisualizer

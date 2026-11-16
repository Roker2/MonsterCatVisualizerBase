// SPDX-License-Identifier: MIT

#pragma once

#include <cmath>
#include <type_traits>

#include "utils/approximate.h"

namespace monsterCatVisualizer::primitives
{

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

} // namespace monsterCatVisualizer::primitives

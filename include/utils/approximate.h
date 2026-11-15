#pragma once

#include <cmath>
#include <type_traits>

#include "utils/defines.h"

namespace monsterCatVisualizer::utils
{

struct Approximate
{
	template <typename T, typename U>
	[[nodiscard]] static constexpr bool equal(T a, U b,
        std::common_type_t<T, U> epsilon = smallNumber) noexcept
	{
		return std::abs(a - b) <= epsilon;
	}
	template <typename T>
	[[nodiscard]] static constexpr bool equalZero(T a,
        T epsilon = smallNumber) noexcept
	{
		return std::abs(a) <= epsilon;
	}

	template <typename T, typename U>
	[[nodiscard]] static constexpr bool greater(T a, U b,
        std::common_type_t<T, U> epsilon = smallNumber) noexcept
	{
		return a - b > epsilon;
	}
	template <typename T>
	[[nodiscard]] static constexpr bool greaterZero(T a,
        T epsilon = smallNumber) noexcept
	{
		return a > epsilon;
	}

	template <typename T, typename U>
	[[nodiscard]] static constexpr bool less(T a, U b,
        std::common_type_t<T, U> epsilon = smallNumber) noexcept
	{
		return b - a > epsilon;
	}
	template <typename T>
	[[nodiscard]] static constexpr bool lessZero(T a,
        T epsilon = smallNumber) noexcept
	{
		return a < -epsilon;
	}

	template <typename T, typename U>
	[[nodiscard]] static constexpr bool greaterEqual(T a, U b,
        std::common_type_t<T, U> epsilon = smallNumber) noexcept
	{
		return a - b >= -epsilon;
	}
	template <typename T>
	[[nodiscard]] static constexpr bool greaterEqualZero(T a,
        T epsilon = smallNumber) noexcept
	{
		return a >= -epsilon;
	}

	template <typename T, typename U>
	[[nodiscard]] static constexpr bool lessEqual(T a, U b,
        std::common_type_t<T, U> epsilon = smallNumber) noexcept
	{
		return b - a >= -epsilon;
	}
	template <typename T>
	[[nodiscard]] static constexpr bool lessEqualZero(T a,
        T epsilon = smallNumber) noexcept
	{
		return a <= epsilon;
	}

	template <typename T, typename U>
    [[nodiscard]] static constexpr bool contains(T a, U min, U max,
		std::common_type_t<T, U> epsilon = smallNumber) noexcept
    {
        return utils::Approximate::greater(a, min, epsilon) &&
            utils::Approximate::less(a, max, epsilon);
    }
};

struct ApproximateEqualComparator
{
    template <typename T, typename U>
	[[nodiscard]] constexpr bool operator()(T a, U b) const
	{
		return Approximate::equal(a, b);
	}
};

struct ApproximateGreaterComparator
{
    template <typename T, typename U>
	[[nodiscard]] constexpr bool operator()(T a, U b) const
	{
		return Approximate::greater(a, b);
	}
};

struct ApproximateLessComparator
{
    template <typename T, typename U>
	[[nodiscard]] constexpr bool operator()(T a, U b) const
	{
		return Approximate::less(a, b);
	}
};

struct ApproximateGreaterEqualComparator
{
    template <typename T, typename U>
	[[nodiscard]] constexpr bool operator()(T a, U b) const
	{
		return Approximate::greaterEqual(a, b);
	}
};

struct ApproximateLessEqualComparator
{
    template <typename T, typename U>
	[[nodiscard]] constexpr bool operator()(T a, U b) const
	{
		return Approximate::lessEqual(a, b);
	}
};

} // namespace monsterCatVisualizer::utils

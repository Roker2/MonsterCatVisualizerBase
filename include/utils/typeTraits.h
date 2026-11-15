#pragma once

#include <type_traits>
#include <utility>

namespace monsterCatVisualizer::utils
{

template <typename F, typename = void>
struct is_null_comparable : std::false_type {};

template <typename F>
struct is_null_comparable<F, std::void_t<
    decltype(static_cast<bool>(std::declval<const F&>() == nullptr))>>
    : std::true_type {};

template <typename T, typename U = T>
inline constexpr bool is_null_comparable_v = is_null_comparable<T, U>::value;


template <typename T, typename U = T, typename = void>
struct is_equality_comparable : std::false_type {};

template <typename T, typename U>
struct is_equality_comparable<T, U, std::void_t<
    decltype(std::declval<const T&>() == std::declval<const U&>()),
    decltype(std::declval<const T&>() != std::declval<const U&>())>>
    : std::true_type {};

template <typename T, typename U = T>
inline constexpr bool is_equality_comparable_v = is_equality_comparable<T, U>::value;

} // namespace monsterCatVisualizer::utils

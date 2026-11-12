#pragma once

#include <type_traits>

namespace monsterCatVisualizer::utils
{

template <typename F, typename = void>
struct is_null_comparable : std::false_type {};

template <typename F>
struct is_null_comparable<F, std::void_t<
    decltype(static_cast<bool>(std::declval<F&>() == nullptr))>>
    : std::true_type {};

} // namespace monsterCatVisualizer::utils

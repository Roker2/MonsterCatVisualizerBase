// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <cstdint>
#include <random>

namespace monsterCatVisualizer::animation
{

class BaseAnimator
{
public:
    virtual ~BaseAnimator() = default;

    virtual void initialize(std::int32_t viewportWidth,
        std::int32_t viewportHeight);
    virtual void initialize(std::size_t desiredCount) = 0;
    virtual void clear() = 0;
    virtual void update(double timeSeconds,
        std::int32_t viewportWidth,
        std::int32_t viewportHeight);
    virtual std::size_t calculateDesiredCount(std::int32_t viewportWidth,
        std::int32_t viewportHeight) const = 0;

protected:
    virtual void removeElements(double timeSeconds,
        std::int32_t viewportWidth,
        std::int32_t viewportHeight) = 0;
    virtual void appendElements(double timeSeconds, std::size_t desiredCount) = 0;

    std::mt19937 m_rng{std::random_device{}()};
};

} // namespace monsterCatVisualizer::animation

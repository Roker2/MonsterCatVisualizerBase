// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <random>

namespace monsterCatVisualizer
{

class BaseAnimator
{
public:
    virtual ~BaseAnimator() = default;

    virtual void initialize(float viewportWidth,
        float viewportHeight);
    virtual void initialize(std::size_t desiredCount) = 0;
    virtual void clear() = 0;
    virtual void update(double timeSeconds,
        float viewportWidth,
        float viewportHeight);
    virtual std::size_t calculateDesiredCount(float viewportWidth,
        float viewportHeight) const = 0;

protected:
    virtual void removeElements(double timeSeconds,
        float viewportWidth,
        float viewportHeight) = 0;
    virtual void appendElements(double timeSeconds, std::size_t desiredCount) = 0;

    std::mt19937 m_rng{std::random_device{}()};
};

} // namespace monsterCatVisualizer

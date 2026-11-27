// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>

namespace monsterCatVisualizer::animation
{

class IAnimator
{
public:
    virtual ~IAnimator() = default;

    virtual void initialize(std::int32_t viewportWidth,
        std::int32_t viewportHeight) = 0;
    virtual void clear() = 0;
    virtual void update(double timeSeconds,
        std::int32_t viewportWidth,
        std::int32_t viewportHeight) = 0;
};

} // namespace monsterCatVisualizer::animation

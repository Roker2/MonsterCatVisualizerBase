// SPDX-License-Identifier: MIT

#include "animation/baseAnimator.h"

namespace monsterCatVisualizer::animation
{

void BaseAnimator::initialize(std::int32_t viewportWidth,
    std::int32_t viewportHeight)
{
    const std::size_t desiredCount = calculateDesiredCount(viewportWidth, viewportHeight);
    initialize(desiredCount);
}

void BaseAnimator::update(double timeSeconds,
    std::int32_t viewportWidth,
    std::int32_t viewportHeight)
{
    const std::size_t desiredCount = calculateDesiredCount(viewportWidth, viewportHeight);
    removeElements(timeSeconds, viewportWidth, viewportHeight, desiredCount);
    appendElements(timeSeconds, viewportWidth, viewportHeight, desiredCount);
}

} // namespace monsterCatVisualizer::animation

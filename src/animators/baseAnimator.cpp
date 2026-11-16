// SPDX-License-Identifier: MIT

#include "animators/baseAnimator.h"

namespace monsterCatVisualizer
{

void BaseAnimator::initialize(float viewportWidth,
    float viewportHeight)
{
    const std::size_t desiredCount = calculateDesiredCount(viewportWidth, viewportHeight);
    initialize(desiredCount);
}

void BaseAnimator::update(double timeSeconds,
    float viewportWidth,
    float viewportHeight)
{
    const std::size_t desiredCount = calculateDesiredCount(viewportWidth, viewportHeight);
    removeElements(timeSeconds, viewportWidth, viewportHeight);
    appendElements(timeSeconds, desiredCount);
}

} // namespace monsterCatVisualizer

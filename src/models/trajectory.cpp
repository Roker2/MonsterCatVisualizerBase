// SPDX-License-Identifier: MIT

#include "models/trajectory.h"

#include "utils/mathUtils.h"

namespace monsterCatVisualizer::models
{

primitives::Vector2<> TrajectoryDescriptor::calculatePosition(double timeSeconds,
    float viewportWidth,
    float viewportHeight) const
{
    return calculatePosition(timeSeconds,
        utils::SpatialMathUtils::viewportToRatio(viewportWidth, viewportHeight));
}

primitives::Vector2<> TrajectoryDescriptor::calculatePosition(double timeSeconds,
    float viewportRatio) const
{
    primitives::Vector2<> directionNVS = utils::SpatialMathUtils::convertNSSToNVS(
        direction, viewportRatio);
    directionNVS.normalize();

    const float offsetMultiplier = static_cast<float>(
        speed * (timeSeconds - startTimeSeconds));
    const primitives::Vector2<> offset = directionNVS * offsetMultiplier;

    return startPosition + offset;
}

} // namespace monsterCatVisualizer::models

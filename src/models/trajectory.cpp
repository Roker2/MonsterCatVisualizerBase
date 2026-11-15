#include "models/trajectory.h"

#include "utils/mathUtils.h"

namespace monsterCatVisualizer
{

Vector2<> TrajectoryDescriptor::calculatePosition(double timeSeconds,
    float viewportWidth,
    float viewportHeight) const
{
    return calculatePosition(timeSeconds,
        utils::SpatialMathUtils::viewportToRatio(viewportWidth, viewportHeight));
}

Vector2<> TrajectoryDescriptor::calculatePosition(double timeSeconds,
    float viewportRatio) const
{
    Vector2<> directionNVS = utils::SpatialMathUtils::convertNSSToNVS(
        direction, viewportRatio);
    directionNVS.normalize();

    const float offsetMultiplier = static_cast<float>(
        speed * (timeSeconds - startTimeSeconds));
    const Vector2<> offset = directionNVS * offsetMultiplier;

    return startPosition + offset;
}

} // namespace monsterCatVisualizer

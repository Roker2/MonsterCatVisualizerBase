#include "utils/mathUtils.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

#include "utils/defines.h"
#include "utils/approximate.h"

namespace monsterCatVisualizer::utils
{

Vector2<> SpatialMathUtils::convertNVSToNSS(Vector2<> position,
    float viewportWidth,
    float viewportHeight)
{
    return convertNVSToNSS(position,
        viewportToRatio(viewportWidth, viewportHeight));
}

Vector2<> SpatialMathUtils::convertNVSToNSS(Vector2<> position,
    float viewportRatio)
{
    if (!std::isfinite(viewportRatio) || Approximate::lessEqualZero(viewportRatio))
        throw std::invalid_argument("SpatialMathUtils: invalid viewport ratio");

    if (viewportRatio >= 1.0f)
    {
        return Vector2<>{ position.x, position.y / viewportRatio };
    }
    return Vector2<>{ position.x * viewportRatio, position.y };
}

Vector2<> SpatialMathUtils::convertNSSToNVS(Vector2<> position,
    float viewportWidth,
    float viewportHeight)
{
    return convertNSSToNVS(position,
        viewportToRatio(viewportWidth, viewportHeight));
}

Vector2<> SpatialMathUtils::convertNSSToNVS(Vector2<> position,
    float viewportRatio)
{
    if (!std::isfinite(viewportRatio) || Approximate::lessEqualZero(viewportRatio))
        throw std::invalid_argument("SpatialMathUtils: invalid viewport ratio");

    if (viewportRatio >= 1.0f)
    {
        return Vector2<>{ position.x, position.y * viewportRatio };
    }
    return Vector2<>{ position.x / viewportRatio, position.y };
}

float SpatialMathUtils::viewportToRatio(float viewportWidth,
    float viewportHeight)
{
    if (!viewportSizeValid(viewportWidth, viewportHeight))
        throw std::invalid_argument("SpatialMathUtils: invalid viewport size");

    return std::max(viewportWidth, minViewportSide) /
        std::max(viewportHeight, minViewportSide);
}

bool SpatialMathUtils::viewportSizeValid(float viewportWidth,
    float viewportHeight)
{
    return std::isfinite(viewportWidth) &&
        std::isfinite(viewportHeight) &&
        Approximate::greaterZero(viewportWidth) &&
        Approximate::greaterZero(viewportHeight);
}

} // namespace monsterCatVisualizer::utils

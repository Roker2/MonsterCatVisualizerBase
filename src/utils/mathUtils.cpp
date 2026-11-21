// SPDX-License-Identifier: MIT

#include "utils/mathUtils.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>

#include "utils/approximate.h"

namespace monsterCatVisualizer::utils
{

std::size_t SpatialMathUtils::calculateDesiredCount(float elementsDensity,
    std::int32_t viewportWidth,
    std::int32_t viewportHeight)
{
    const float viewportRatio = viewportToRatio(
        viewportWidth, viewportHeight);
    return calculateDesiredCount(elementsDensity, viewportRatio);
}

std::size_t SpatialMathUtils::calculateDesiredCount(float elementsDensity,
    float viewportRatio)
{
    const float viewportMultiplier = std::max(viewportRatio, 1.0f / viewportRatio);
    const float count = std::round(elementsDensity * viewportMultiplier);

    if (!std::isfinite(count) ||
        count >= static_cast<float>(std::numeric_limits<std::size_t>::max()))
        throw std::overflow_error("ContainerAnimator: desired element count is too large");

    return static_cast<std::size_t>(count);
}

primitives::Vector2<> SpatialMathUtils::convertNVSToNSS(
    primitives::Vector2<> position,
    std::int32_t viewportWidth,
    std::int32_t viewportHeight)
{
    return convertNVSToNSS(position,
        viewportToRatio(viewportWidth, viewportHeight));
}

primitives::Vector2<> SpatialMathUtils::convertNVSToNSS(
    primitives::Vector2<> position,
    float viewportRatio)
{
    if (!std::isfinite(viewportRatio) || Approximate::lessEqualZero(viewportRatio))
        throw std::invalid_argument("SpatialMathUtils: invalid viewport ratio");

    if (viewportRatio >= 1.0f)
        return primitives::Vector2<>{ position.x, position.y / viewportRatio };
    return primitives::Vector2<>{ position.x * viewportRatio, position.y };
}

primitives::Vector2<> SpatialMathUtils::convertNSSToNVS(
    primitives::Vector2<> position,
    std::int32_t viewportWidth,
    std::int32_t viewportHeight)
{
    return convertNSSToNVS(position,
        viewportToRatio(viewportWidth, viewportHeight));
}

primitives::Vector2<> SpatialMathUtils::convertNSSToNVS(
    primitives::Vector2<> position,
    float viewportRatio)
{
    if (!std::isfinite(viewportRatio) || Approximate::lessEqualZero(viewportRatio))
        throw std::invalid_argument("SpatialMathUtils: invalid viewport ratio");

    if (viewportRatio >= 1.0f)
        return primitives::Vector2<>{ position.x, position.y * viewportRatio };
    return primitives::Vector2<>{ position.x / viewportRatio, position.y };
}

float SpatialMathUtils::viewportToRatio(std::int32_t viewportWidth,
    std::int32_t viewportHeight)
{
    if (!viewportSizeValid(viewportWidth, viewportHeight))
        throw std::invalid_argument("SpatialMathUtils: invalid viewport size");

    return static_cast<float>(viewportWidth) / viewportHeight;
}

bool SpatialMathUtils::viewportSizeValid(std::int32_t viewportWidth,
    std::int32_t viewportHeight)
{
    return viewportWidth > 0 && viewportHeight > 0;
}

} // namespace monsterCatVisualizer::utils

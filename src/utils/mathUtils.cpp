// SPDX-License-Identifier: MIT

#include "utils/mathUtils.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

#include "utils/approximate.h"

namespace monsterCatVisualizer::utils
{

std::size_t SpatialMathUtils::calculateDesiredCount(float elementsDensity,
    std::int32_t /* viewportWidth */,
    std::int32_t /* viewportHeight */)
{
    return static_cast<std::size_t>(elementsDensity);
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

primitives::Vector2<> SpatialMathUtils::convertNSSToSS(
    primitives::Vector2<> positionNSS,
    std::int32_t viewportWidth, std::int32_t viewportHeight)
{
    if (!viewportSizeValid(viewportWidth, viewportHeight))
        throw std::invalid_argument("SpatialMathUtils: invalid viewport size");

    const std::int32_t maxDimension = std::max(viewportWidth, viewportHeight);
    return positionNSS * maxDimension;
}

float SpatialMathUtils::convertNSSToSS(float valueNSS,
    std::int32_t viewportWidth, std::int32_t viewportHeight)
{
    if (!viewportSizeValid(viewportWidth, viewportHeight))
        throw std::invalid_argument("SpatialMathUtils: invalid viewport size");

    const std::int32_t maxDimension = std::max(viewportWidth, viewportHeight);
    return valueNSS * maxDimension;
}

primitives::Vector2<> SpatialMathUtils::convertSSToNSS(
    primitives::Vector2<> positionSS,
    std::int32_t viewportWidth, std::int32_t viewportHeight)
{
    if (!viewportSizeValid(viewportWidth, viewportHeight))
        throw std::invalid_argument("SpatialMathUtils: invalid viewport size");

    const std::int32_t maxDimension = std::max(viewportWidth, viewportHeight);
    return positionSS / maxDimension;
}

float SpatialMathUtils::convertSSToNSS(float valueSS,
    std::int32_t viewportWidth, std::int32_t viewportHeight)
{
    if (!viewportSizeValid(viewportWidth, viewportHeight))
        throw std::invalid_argument("SpatialMathUtils: invalid viewport size");

    const std::int32_t maxDimension = std::max(viewportWidth, viewportHeight);
    return valueSS / maxDimension;
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

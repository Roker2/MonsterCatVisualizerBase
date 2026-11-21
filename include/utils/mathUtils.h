#pragma once

#include "primitives/vector2.h"

namespace monsterCatVisualizer::utils
{

struct SpatialMathUtils
{
    // NVS - viewport space in relative coordinates.
    // NSS - screen space in relative coordinates.
    // E.g. at 2:1, NVS [0, 1] x [0, 1] maps to NSS [0, 1] x [0, 0.5].
    // Multiply NSS coordinates by max(viewportWidth, viewportHeight) to get pixels.
    // The viewport ratio must be finite and positive.

    static std::size_t calculateDesiredCount(float elementsDensity,
        std::int32_t viewportWidth,
        std::int32_t viewportHeight);
    static std::size_t calculateDesiredCount(float elementsDensity,
        float viewportRatio);

    /** Convert position from NVS to NSS */
    static primitives::Vector2<> convertNVSToNSS(
        primitives::Vector2<> positionNVS,
        std::int32_t viewportWidth,
        std::int32_t viewportHeight);
    /** Convert position from NVS to NSS */
    static primitives::Vector2<> convertNVSToNSS(
        primitives::Vector2<> positionNVS,
        float viewportRatio);

    /** Convert position from NSS to NVS */
    static primitives::Vector2<> convertNSSToNVS(
        primitives::Vector2<> positionNSS,
        std::int32_t viewportWidth,
        std::int32_t viewportHeight);
    /** Convert position from NSS to NVS */
    static primitives::Vector2<> convertNSSToNVS(
        primitives::Vector2<> positionNSS,
        float viewportRatio);

    static float viewportToRatio(std::int32_t viewportWidth,
        std::int32_t viewportHeight);
    static bool viewportSizeValid(std::int32_t viewportWidth,
        std::int32_t viewportHeight);
};

} // namespace monsterCatVisualizer::utils

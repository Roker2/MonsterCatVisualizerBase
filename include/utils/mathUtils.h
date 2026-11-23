#pragma once

#include <cstddef>
#include <cstdint>

#include "primitives/vector2.h"

namespace monsterCatVisualizer::utils
{

struct SpatialMathUtils
{
    // SS - Screen Space, measured in framebuffer pixels.
    // NVS - viewport space in relative coordinates.
    // NSS - screen space in relative coordinates.
    // NSS and SS share the same origin and axis directions.
    // E.g. at 2:1, NVS [0, 1] x [0, 1] maps to NSS [0, 1] x [0, 0.5].
    // Convert NSS to SS by multiplying by max(viewportWidth, viewportHeight) to get pixels.
    // The conversion applies equally to positions, sizes and distances.
    // The viewport ratio must be finite and positive.

    static std::size_t calculateDesiredCount(float elementsDensity,
        std::int32_t viewportWidth,
        std::int32_t viewportHeight);

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

    /** Convert position from NSS to SS */
    static primitives::Vector2<> convertNSSToSS(
        primitives::Vector2<> positionNSS,
        std::int32_t viewportWidth, std::int32_t viewportHeight);
    /** Convert scalar from NSS to SS */
    static float convertNSSToSS(float valueNSS,
        std::int32_t viewportWidth, std::int32_t viewportHeight);

    /** Convert position from SS to NSS */
    static primitives::Vector2<> convertSSToNSS(
        primitives::Vector2<> positionSS,
        std::int32_t viewportWidth, std::int32_t viewportHeight);
    /** Convert scalar from SS to NSS */
    static float convertSSToNSS(float valueSS,
        std::int32_t viewportWidth, std::int32_t viewportHeight);

    static float viewportToRatio(std::int32_t viewportWidth,
        std::int32_t viewportHeight);
    static bool viewportSizeValid(std::int32_t viewportWidth,
        std::int32_t viewportHeight);
};

} // namespace monsterCatVisualizer::utils

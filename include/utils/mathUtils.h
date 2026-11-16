#pragma once

#include "models/vector2.h"

namespace monsterCatVisualizer::utils
{

struct SpatialMathUtils
{
    // NVS - viewport space in relative coordinates.
    // NSS - screen space in relative coordinates.
    // E.g. at 2:1, NVS [0, 1] x [0, 1] maps to NSS [0,1] x [0, 0.5].
    // Multiply NSS coordinates by max(viewportWidth, viewportHeight) to get pixels.
    // The viewport ratio must be finite and positive.

    /** Convert position from NVS to NSS */
    static Vector2<> convertNVSToNSS(Vector2<> positionNVS,
        float viewportWidth,
        float viewportHeight);
    /** Convert position from NVS to NSS */
    static Vector2<> convertNVSToNSS(Vector2<> positionNVS,
        float viewportRatio);

    /** Convert position from NSS to NVS */
    static Vector2<> convertNSSToNVS(Vector2<> positionNSS,
        float viewportWidth,
        float viewportHeight);
    /** Convert position from NSS to NVS */
    static Vector2<> convertNSSToNVS(Vector2<> positionNSS,
        float viewportRatio);

    static float viewportToRatio(float viewportWidth,
        float viewportHeight);
    static bool viewportSizeValid(float viewportWidth,
        float viewportHeight);
};

} // namespace monsterCatVisualizer::utils

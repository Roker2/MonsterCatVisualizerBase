// SPDX-License-Identifier: MIT

#pragma once

#include "models/vector2.h"

namespace monsterCatVisualizer
{

struct TrajectoryDescriptor
{
    /** Calculate current position in NVS */
    Vector2<> calculatePosition(double timeSeconds,
        float viewportWidth,
        float viewportHeight) const;
    /** Calculate current position in NVS */
    Vector2<> calculatePosition(double timeSeconds,
        float viewportRatio) const;

    /** Start position in NVS */
    Vector2<> startPosition{};
    /** Movement direction in NSS */
    Vector2<> direction{};
    /** Speed in NVS */
    float speed{};
    double startTimeSeconds{};
};

} // namespace monsterCatVisualizer

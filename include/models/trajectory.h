// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>

#include "primitives/vector2.h"

namespace monsterCatVisualizer::models
{

struct TrajectoryDescriptor
{
    /** Calculate current position in NVS */
    primitives::Vector2<> calculatePosition(double timeSeconds,
        std::int32_t viewportWidth,
        std::int32_t viewportHeight) const;
    /** Calculate current position in NVS */
    primitives::Vector2<> calculatePosition(double timeSeconds,
        float viewportRatio) const;

    /** Start position in NVS */
    primitives::Vector2<> startPosition{};
    /** Movement direction in NSS */
    primitives::Vector2<> direction{};
    /** Speed in NVS */
    float speed{};
    double startTimeSeconds{};
};

} // namespace monsterCatVisualizer::models

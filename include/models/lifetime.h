// SPDX-License-Identifier: MIT

#pragma once

#include <cmath>

#include "utils/approximate.h"

namespace monsterCatVisualizer::models
{

struct LifetimeDescriptor
{
    inline bool isValid() const
    {
        return std::isfinite(lifetimeSeconds) &&
            std::isfinite(fadeInSeconds) &&
            std::isfinite(fadeOutSeconds) &&
            utils::Approximate::greater(
            lifetimeSeconds, fadeInSeconds, fadeOutSeconds);
    }

    /** Whole lifetime duration */
    double lifetimeSeconds{};
    /** Fade-in duration */
    double fadeInSeconds{};
    /** Fade-out duration */
    double fadeOutSeconds{};
};

} // namespace monsterCatVisualizer::models

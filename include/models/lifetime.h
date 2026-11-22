// SPDX-License-Identifier: MIT

#pragma once

namespace monsterCatVisualizer::models
{

struct LifetimeDescriptor
{
    /** Whole lifetime duration */
    double lifetimeSeconds{};
    /** Fade-in duration */
    double fadeInSeconds{};
    /** Fade-out duration */
    double fadeOutSeconds{};
};

} // namespace monsterCatVisualizer::models

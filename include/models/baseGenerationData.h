// SPDX-License-Identifier: MIT

#pragma once

#include "utils/approximate.h"
#include "primitives/minMax.h"
#include "primitives/color.h"

namespace monsterCatVisualizer::models
{

struct BaseGenerationDescriptor
{
    static constexpr primitives::MinMax<float> floatValidator{0.0f, 1.0f};

    bool isValid() const
    {
        const bool elementsDensityValid = elementsDensity > utils::elementsDensityMin;
        const bool speedValid = utils::Approximate::less(speed.min, speed.max) &&
            floatValidator.contains(speed.min, false) &&
            floatValidator.contains(speed.max, false);

        const bool lifetimeSecondsValid = utils::Approximate::less(lifetimeSeconds.min, lifetimeSeconds.max) &&
            utils::Approximate::greaterZero(lifetimeSeconds.min) &&
            utils::Approximate::greaterZero(lifetimeSeconds.max) &&
            utils::Approximate::greater(lifetimeSeconds.min, fadeInSeconds.max + fadeOutSeconds.max);
        const bool fadeInSecondsValid = utils::Approximate::less(fadeInSeconds.min, fadeInSeconds.max) &&
            utils::Approximate::greaterZero(fadeInSeconds.min) &&
            utils::Approximate::greaterZero(fadeInSeconds.max);
        const bool fadeOutSecondsValid = utils::Approximate::less(fadeOutSeconds.min, fadeOutSeconds.max) &&
            utils::Approximate::greaterZero(fadeOutSeconds.min) &&
            utils::Approximate::greaterZero(fadeOutSeconds.max);

        return elementsDensityValid && speedValid &&
            lifetimeSecondsValid && fadeInSecondsValid && fadeOutSecondsValid;
    }

    static bool isColorRGBAValid(
        const primitives::MinMax<primitives::ColorRGBA<>>& color)
    {
        return color.min.r <= color.max.r &&
            color.min.g <= color.max.g &&
            color.min.b <= color.max.b &&
            utils::Approximate::less(color.min.a, color.max.a) &&
            floatValidator.contains(color.min.a, true) &&
            floatValidator.contains(color.max.a, true);
    }

    float elementsDensity{};
    primitives::MinMax<float> speed{};
    primitives::MinMax<double> lifetimeSeconds{};
    primitives::MinMax<double> fadeInSeconds{};
    primitives::MinMax<double> fadeOutSeconds{};
};

} // namespace monsterCatVisualizer::models

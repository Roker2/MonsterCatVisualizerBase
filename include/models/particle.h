// Based on particles.js from https://github.com/kaischallenberg/monstercatVisualizer
// Copyright (c) 2025 Kai Schallenberg
// Copyright (c) 2026 Dzmitry Minko
// SPDX-License-Identifier: MIT

#pragma once

#include <cmath>

#include "utils/approximate.h"
#include "models/minMax.h"
#include "models/color.h"
#include "models/trajectory.h"

namespace monsterCatVisualizer
{

struct ParticleGenerationDescriptor
{
    static constexpr float elementsDensityMin = 1.0f;
    static constexpr MinMax<float> floatValidator{0.0f, 1.0f};

    bool isValid() const
    {
        const bool elementsDensityValid = std::isfinite(elementsDensity) &&
            elementsDensity > elementsDensityMin;
        const bool speedValid = utils::Approximate::less(speed.min, speed.max) &&
            floatValidator.contains(speed.min, false) &&
            floatValidator.contains(speed.max, false);
        const bool radiusValid = utils::Approximate::less(radius.min, radius.max) &&
            floatValidator.contains(radius.min, false) &&
            floatValidator.contains(radius.max, false);
        const bool colorValid =
            color.min.r <= color.max.r &&
            color.min.g <= color.max.g &&
            color.min.b <= color.max.b &&
            utils::Approximate::less(color.min.a, color.max.a) &&
            floatValidator.contains(color.min.a, true) &&
            floatValidator.contains(color.max.a, true);

        return elementsDensityValid && speedValid && radiusValid && colorValid;
    }

    float elementsDensity{};

    MinMax<float> speed{};
    MinMax<float> radius{};
    MinMax<ColorRGBA<>> color{};
};

struct Particle
{
    struct GeometryDescriptor
    {
        float radius{};
    };

    struct VisualDescriptor
    {
        ColorRGBA<> color{};
    };

    TrajectoryDescriptor trajectory{};
    GeometryDescriptor geometry{};
    VisualDescriptor visual{};
};

} // namespace monsterCatVisualizer

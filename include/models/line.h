// Based on particles.js from https://github.com/kaischallenberg/monstercatVisualizer
// Copyright (c) 2025 Kai Schallenberg
// Copyright (c) 2026 Dzmitry Minko
// SPDX-License-Identifier: MIT

#pragma once

#include <cmath>

#include "utils/approximate.h"
#include "primitives/minMax.h"
#include "primitives/color.h"
#include "models/trajectory.h"

namespace monsterCatVisualizer::models
{

struct LineGenerationDescriptor
{
    static constexpr primitives::MinMax<float> floatValidator{0.0f, 1.0f};

    bool isValid() const
    {
        const bool elementsDensityValid = std::isfinite(elementsDensity) &&
            elementsDensity > utils::elementsDensityMin;
        const bool speedValid = utils::Approximate::less(speed.min, speed.max) &&
            floatValidator.contains(speed.min, false) &&
            floatValidator.contains(speed.max, false);
        const bool lengthValid = utils::Approximate::less(length.min, length.max) &&
            floatValidator.contains(length.min, false) &&
            floatValidator.contains(length.max, false);
        const bool lineWidthValid = floatValidator.contains(lineWidth, false);
        const bool colorValid =
            color.min.r <= color.max.r &&
            color.min.g <= color.max.g &&
            color.min.b <= color.max.b &&
            utils::Approximate::less(color.min.a, color.max.a) &&
            floatValidator.contains(color.min.a, true) &&
            floatValidator.contains(color.max.a, true);

        return elementsDensityValid && speedValid && lengthValid &&
            lineWidthValid && colorValid;
    }

    float elementsDensity{};

    primitives::MinMax<float> speed{};
    primitives::MinMax<float> length{};
    float lineWidth{};
    primitives::MinMax<primitives::ColorRGBA<>> color{};
};

struct Line
{
    using GenerationDescriptor = LineGenerationDescriptor;

    struct GeometryDescriptor
    {
        /** Normalized direction along the line in NSS. */
        primitives::Vector2<> direction{1.0f, 0.0f};
        float length{};
        float lineWidth{};
    };

    struct VisualDescriptor
    {
        primitives::ColorRGBA<> color{};
    };

    TrajectoryDescriptor trajectory{};
    GeometryDescriptor geometry{};
    VisualDescriptor visual{};
};

} // namespace monsterCatVisualizer::models

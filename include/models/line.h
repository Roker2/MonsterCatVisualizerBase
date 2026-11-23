// Based on particles.js from https://github.com/kaischallenberg/monstercatVisualizer
// Copyright (c) 2025 Kai Schallenberg
// Copyright (c) 2026 Dzmitry Minko
// SPDX-License-Identifier: MIT

#pragma once

#include "models/baseGenerationData.h"
#include "models/lifetime.h"
#include "models/trajectory.h"

namespace monsterCatVisualizer::models
{

struct LineGenerationDescriptor : public BaseGenerationDescriptor
{
    bool isValid() const
    {
        if (!BaseGenerationDescriptor::isValid())
            return false;

        const bool lengthValid = utils::Approximate::less(length.min, length.max) &&
            floatValidator.contains(length.min, false) &&
            floatValidator.contains(length.max, false);
        const bool lineWidthValid = floatValidator.contains(lineWidth, false);
        const bool colorValid = isColorRGBAValid(color);

        return lengthValid && lineWidthValid && colorValid;
    }

    primitives::MinMax<float> length{};
    float lineWidth{};
    primitives::MinMax<primitives::ColorRGBA<>> color{};
};

struct Line
{
    using GenerationDescriptor = LineGenerationDescriptor;

    struct GeometryDescriptor
    {
        /** Length in NSS */
        float length{};
        /** Line width in NSS */
        float lineWidth{};
    };

    struct VisualDescriptor
    {
        primitives::ColorRGBA<> color{};
    };

    TrajectoryDescriptor trajectory{};
    LifetimeDescriptor lifetime{};
    GeometryDescriptor geometry{};
    VisualDescriptor visual{};
};

} // namespace monsterCatVisualizer::models

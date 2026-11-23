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

struct CloudGenerationDescriptor : public BaseGenerationDescriptor
{
    bool isValid() const
    {
        if (!BaseGenerationDescriptor::isValid())
            return false;

        const bool radiusValid = utils::Approximate::less(radius.min, radius.max) &&
            floatValidator.contains(radius.min, false) &&
            floatValidator.contains(radius.max, false);
        const bool colorValid = isColorRGBAValid(color);
        const bool themeColorProbabilityValid =
            floatValidator.contains(themeColorProbability, true);

        return radiusValid && colorValid && themeColorProbabilityValid;
    }

    primitives::MinMax<float> radius{};
    primitives::MinMax<primitives::ColorRGBA<>> color{};
    float themeColorProbability{};
};

struct Cloud
{
    using GenerationDescriptor = CloudGenerationDescriptor;

    struct GeometryDescriptor
    {
        /** Radius in NSS */
        float radius{};
    };

    struct VisualDescriptor
    {
        primitives::ColorRGBA<> color{};
        bool isThemeColored{};
    };

    TrajectoryDescriptor trajectory{};
    LifetimeDescriptor lifetime{};
    GeometryDescriptor geometry{};
    VisualDescriptor visual{};
};

} // namespace monsterCatVisualizer::models

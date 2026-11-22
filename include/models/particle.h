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

struct ParticleGenerationDescriptor : public BaseGenerationDescriptor
{
    bool isValid() const
    {
        if (!BaseGenerationDescriptor::isValid())
            return false;

        const bool radiusValid = utils::Approximate::less(radius.min, radius.max) &&
            floatValidator.contains(radius.min, false) &&
            floatValidator.contains(radius.max, false);
        const bool colorValid = isColorRGBAValid(color);

        return radiusValid && colorValid;
    }

    primitives::MinMax<float> radius{};
    primitives::MinMax<primitives::ColorRGBA<>> color{};
};

struct Particle
{
    using GenerationDescriptor = ParticleGenerationDescriptor;

    static constexpr float minRadiusPx = 2.5f;

    struct GeometryDescriptor
    {
        /** Radius in NSS */
        float radius{};
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

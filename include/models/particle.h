// Based on particles.js from https://github.com/kaischallenberg/monstercatVisualizer
// Copyright (c) 2025 Kai Schallenberg
// Copyright (c) 2026 Dzmitry Minko
// SPDX-License-Identifier: MIT

#pragma once

#include "models/primitives.h"

namespace monsterCatVisualizer
{

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

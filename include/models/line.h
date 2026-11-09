// Based on particles.js from https://github.com/kaischallenberg/monstercatVisualizer
// Copyright (c) 2025 Kai Schallenberg
// Copyright (c) 2026 Dzmitry Minko
// SPDX-License-Identifier: MIT

#pragma once

#include "models/primitives.h"

namespace monsterCatVisualizer
{

struct Line
{
    struct GeometryDescriptor
    {
        /** Normalized direction along the line. */
        Vector2<> direction{1.0f, 0.0f};
        float length{};
        float lineWidth{};
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

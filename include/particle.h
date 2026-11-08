// Based on particles.js from https://github.com/kaischallenberg/monstercatVisualizer
// Copyright (c) 2025 Kai Schallenberg
// Copyright (c) 2026 Dzmitry Minko
// SPDX-License-Identifier: MIT

#pragma once

#include "primitives.h"

namespace monsterCatVisualizer
{

struct Particle
{
    Vector2<> position{};
    Vector2<> velocity{};
    float radius{};
    ColorRGBA<> color{};
};

} // namespace monsterCatVisualizer

// Based on particles.js from https://github.com/kaischallenberg/monstercatVisualizer
// Copyright (c) 2025 Kai Schallenberg
// Copyright (c) 2026 Dzmitry Minko
// SPDX-License-Identifier: MIT

#pragma once

#include <random>

#include "animation/containerAnimator.h"
#include "models/line.h"

namespace monsterCatVisualizer::animation
{

class LinesAnimator : public ContainerAnimator<models::Line>
{
public:
    static constexpr std::size_t appendLimit = 25;

    using LineGenerationDescriptor = models::LineGenerationDescriptor;
    using Line = models::Line;

    struct GenerationDescriptor
    {
        using DistF = std::uniform_real_distribution<float>;

        explicit GenerationDescriptor(const LineGenerationDescriptor& source);

        DistF length{};
        float lineWidth{};
    };

    using GeometryDescriptor = Line::GeometryDescriptor;
    using VisualDescriptor = Line::VisualDescriptor;
    using Storage = ContainerAnimator<Line>::Storage;
    using StoragePtr = ContainerAnimator<Line>::StoragePtr;

    LinesAnimator(StoragePtr storagePtr,
        const LineGenerationDescriptor& lineGenerationDescriptor);

protected:
    void removeElements(double timeSeconds,
        float viewportWidth,
        float viewportHeight) override;
    void appendElements(double timeSeconds, std::size_t desiredCount) override;

    Line generateElement(double startTimeSeconds) override;

    GenerationDescriptor m_generationDescriptor;
};

} // namespace monsterCatVisualizer::animation

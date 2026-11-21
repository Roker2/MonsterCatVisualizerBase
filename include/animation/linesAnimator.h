// Based on particles.js from https://github.com/kaischallenberg/monstercatVisualizer
// Copyright (c) 2025 Kai Schallenberg
// Copyright (c) 2026 Dzmitry Minko
// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>
#include <random>

#include "models/line.h"
#include "animation/containerAnimator.h"

namespace monsterCatVisualizer::animation
{

class LinesAnimator : public ContainerAnimator<models::Line>
{
public:
    static constexpr std::size_t appendLimit = 50;

    struct GenerationDescriptor
    {
        using DistF = std::uniform_real_distribution<float>;

        explicit GenerationDescriptor(const SourceGenerationDescriptor& source);

        DistF length{};
        float lineWidth{};
    };

    LinesAnimator(StoragePtr storagePtr,
        const SourceGenerationDescriptor& source);

protected:
    void removeElements(double timeSeconds,
        std::int32_t viewportWidth,
        std::int32_t viewportHeight) override;
    void appendElements(double timeSeconds, std::size_t desiredCount) override;

    DataModel generateElement(double startTimeSeconds) override;

    GenerationDescriptor m_generationDescriptor;
};

} // namespace monsterCatVisualizer::animation

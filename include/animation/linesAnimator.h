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
    static constexpr std::size_t appendLimit = 10;

    struct GenerationDescriptor
    {
        using DistF = std::uniform_real_distribution<float>;

        explicit GenerationDescriptor(const SourceGenerationDescriptor& source);

        DistF length{};
        float lineWidth{};

        DistF scalarPosition{0.1f, 0.9f};
    };

    LinesAnimator(const SourceGenerationDescriptor& source,
        StoragePtr storagePtr);

    /**
     * An update is not atomic across the patch check, removal, and append.
     * Intended for one producer and one consumer.
     * No other thread may mutate this storage while the producer is updating it.
     */
    void update(double timeSeconds,
        std::int32_t viewportWidth,
        std::int32_t viewportHeight) override;

protected:
    DataModel generateElement(double timeSeconds,
        std::int32_t viewportWidth,
        std::int32_t viewportHeight) override;

    models::TrajectoryDescriptor generateCustomTrajectory(
        double timeSeconds,
        std::int32_t viewportWidth,
        std::int32_t viewportHeight,
        primitives::Vector2<> direction,
        float length);

    GenerationDescriptor m_generationDescriptor;
    bool m_generateInitial{true};
};

} // namespace monsterCatVisualizer::animation

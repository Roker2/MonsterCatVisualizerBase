// Based on particles.js from https://github.com/kaischallenberg/monstercatVisualizer
// Copyright (c) 2025 Kai Schallenberg
// Copyright (c) 2026 Dzmitry Minko
// SPDX-License-Identifier: MIT

#pragma once

#include <array>
#include <cstdint>
#include <random>

#include "models/particle.h"
#include "animation/containerAnimator.h"

namespace monsterCatVisualizer::animation
{

class ParticlesAnimator : public ContainerAnimator<models::Particle>
{
public:
    static constexpr std::size_t appendLimit = 100;

    struct GenerationDescriptor
    {
        static constexpr std::array<float, 6> boundaries
        {
            0.0f,
            0.1f,
            0.2f,
            0.3f,
            0.55f,
            1.0f
        };

        static constexpr std::array<float, 6> weights
        {
            0.2f, 2.0f, 8.0f, 2.5f, 0.4f, 0.0f
        };

        using DistPWF = std::piecewise_linear_distribution<float>;

        explicit GenerationDescriptor(const SourceGenerationDescriptor& source);

        DistPWF radius{};
    };

    ParticlesAnimator(StoragePtr storagePtr,
        const SourceGenerationDescriptor& source);

protected:
    void removeElements(double timeSeconds,
        std::int32_t viewportWidth,
        std::int32_t viewportHeight,
        std::size_t desiredCount) override;
    void appendElements(double timeSeconds,
        std::int32_t viewportWidth,
        std::int32_t viewportHeight,
        std::size_t desiredCount) override;

    DataModel generateElement(double timeSeconds,
        std::int32_t viewportWidth,
        std::int32_t viewportHeight) override;

    GenerationDescriptor m_generationDescriptor;
};

} // namespace monsterCatVisualizer::animation

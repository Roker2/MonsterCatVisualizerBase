// Based on particles.js from https://github.com/kaischallenberg/monstercatVisualizer
// Copyright (c) 2025 Kai Schallenberg
// Copyright (c) 2026 Dzmitry Minko
// SPDX-License-Identifier: MIT

#pragma once

#include <random>

#include "animators/containerAnimator.h"
#include "models/particle.h"

namespace monsterCatVisualizer
{

class ParticlesAnimator : public ContainerAnimator<Particle>
{
public:
    static constexpr std::size_t appendLimit = 250;

    struct GenerationDescriptor
    {
        using DistF = std::uniform_real_distribution<float>;

        explicit GenerationDescriptor(const ParticleGenerationDescriptor& source);

        DistF radius{};
    };

    using GeometryDescriptor = Particle::GeometryDescriptor;
    using VisualDescriptor = Particle::VisualDescriptor;
    using Storage = ContainerAnimator<Particle>::Storage;
    using StoragePtr = ContainerAnimator<Particle>::StoragePtr;

    ParticlesAnimator(StoragePtr storagePtr,
        const ParticleGenerationDescriptor& particleGenerationDescriptor);

protected:
    void removeElements(double timeSeconds,
        float viewportWidth,
        float viewportHeight) override;
    void appendElements(double timeSeconds, std::size_t desiredCount) override;

    Particle generateElement(double startTimeSeconds) override;

    GenerationDescriptor m_generationDescriptor;
};

} // namespace monsterCatVisualizer

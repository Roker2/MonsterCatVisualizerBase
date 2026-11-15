// Based on particles.js from https://github.com/kaischallenberg/monstercatVisualizer
// Copyright (c) 2025 Kai Schallenberg
// Copyright (c) 2026 Dzmitry Minko
// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>
#include <memory>
#include <random>

#include "utils/defines.h"
#include "models/particle.h"
#include "models/container.h"
#include "animators/baseAnimator.h"

namespace monsterCatVisualizer
{

class ParticlesAnimator : public BaseAnimator
{
public:
    static constexpr std::size_t appendLimit = 250;

    struct GenerationDescriptor
    {
        using DistF = std::uniform_real_distribution<float>;
        using DistU16 = std::uniform_int_distribution<std::uint16_t>;

        GenerationDescriptor(const ParticleGenerationDescriptor& particleGenerationDescriptor);

        float elementsDensity{};

        DistF position{0.0f, 1.0f};
        DistF angle{0.0f, utils::twoPi};
        DistF speed{};

        DistF radius{};

        DistU16 colorR{};
        DistU16 colorG{};
        DistU16 colorB{};
        DistF colorA{};
    };

    using GeometryDescriptor = Particle::GeometryDescriptor;
    using VisualDescriptor = Particle::VisualDescriptor;
    using Storage = Container<Particle>;
    using StoragePtr = std::shared_ptr<Storage>;

    ParticlesAnimator(StoragePtr storagePtr,
        const ParticleGenerationDescriptor& particleGenerationDescriptor);

    StoragePtr getStorage() const;

    using BaseAnimator::initialize;
    void initialize(std::size_t desiredCount) override;
    void clear() override;
    void update(double timeSeconds,
        float viewportWidth,
        float viewportHeight) override;
    std::size_t calculateDesiredCount(float viewportWidth,
        float viewportHeight) const override;

protected:
    void removeElements(double timeSeconds,
        float viewportWidth,
        float viewportHeight) override;
    void appendElements(double timeSeconds, std::size_t desiredCount) override;

    virtual Particle generateParticle(double startTimeSeconds);

    StoragePtr m_storagePtr;
    GenerationDescriptor m_generationDescriptor;
};

} // namespace monsterCatVisualizer
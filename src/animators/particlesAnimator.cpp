// Based on particles.js from https://github.com/kaischallenberg/monstercatVisualizer
// Copyright (c) 2025 Kai Schallenberg
// Copyright (c) 2026 Dzmitry Minko
// SPDX-License-Identifier: MIT

#include "animators/particlesAnimator.h"

namespace monsterCatVisualizer
{

ParticlesAnimator::GenerationDescriptor::GenerationDescriptor(
    const ParticleGenerationDescriptor& source)
    : radius(source.radius.min, source.radius.max)
{
}

ParticlesAnimator::ParticlesAnimator(StoragePtr storagePtr,
    const ParticleGenerationDescriptor& particleGenerationDescriptor)
    : ContainerAnimator<Particle>(std::move(storagePtr), particleGenerationDescriptor),
      m_generationDescriptor(particleGenerationDescriptor)
{
}

void ParticlesAnimator::removeElements(double timeSeconds,
    float viewportWidth,
    float viewportHeight)
{
    removeOutsideViewport(timeSeconds, viewportWidth, viewportHeight,
        [](const Particle& particle) { return particle.geometry.radius; });
}

void ParticlesAnimator::appendElements(double timeSeconds, std::size_t desiredCount)
{
    appendGeneratedElements(timeSeconds, desiredCount, appendLimit);
}

Particle ParticlesAnimator::generateElement(double startTimeSeconds)
{
    const Vector2<> direction = generateDirection();
    const ColorRGBA<> color = generateColor();

    return Particle
    {
        generateTrajectory(startTimeSeconds, direction),
        GeometryDescriptor
        {
            m_generationDescriptor.radius(m_rng)
        },
        VisualDescriptor
        {
            color
        }
    };
}

} // namespace monsterCatVisualizer

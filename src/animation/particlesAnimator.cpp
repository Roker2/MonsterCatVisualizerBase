// Based on particles.js from https://github.com/kaischallenberg/monstercatVisualizer
// Copyright (c) 2025 Kai Schallenberg
// Copyright (c) 2026 Dzmitry Minko
// SPDX-License-Identifier: MIT

#include "animation/particlesAnimator.h"

namespace monsterCatVisualizer::animation
{

ParticlesAnimator::GenerationDescriptor::GenerationDescriptor(
    const SourceGenerationDescriptor& source)
    : radius(source.radius.min, source.radius.max)
{
}

ParticlesAnimator::ParticlesAnimator(StoragePtr storagePtr,
    const SourceGenerationDescriptor& source)
    : ContainerAnimator<DataModel>(std::move(storagePtr), source),
      m_generationDescriptor(source)
{
}

void ParticlesAnimator::removeElements(double timeSeconds,
    std::int32_t viewportWidth,
    std::int32_t viewportHeight)
{
    removeOutsideViewport(timeSeconds, viewportWidth, viewportHeight,
        [](const DataModel& particle) { return particle.geometry.radius; });
}

void ParticlesAnimator::appendElements(double timeSeconds, std::size_t desiredCount)
{
    appendGeneratedElements(timeSeconds, desiredCount, appendLimit);
}

ParticlesAnimator::DataModel ParticlesAnimator::generateElement(double startTimeSeconds)
{
    const primitives::Vector2<> direction = generateDirection();
    const primitives::ColorRGBA<> color = generateColor();

    return DataModel
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

} // namespace monsterCatVisualizer::animation

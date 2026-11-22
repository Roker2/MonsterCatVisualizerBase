// Based on particles.js from https://github.com/kaischallenberg/monstercatVisualizer
// Copyright (c) 2025 Kai Schallenberg
// Copyright (c) 2026 Dzmitry Minko
// SPDX-License-Identifier: MIT

#include "animation/particlesAnimator.h"

namespace
{

template <typename T>
std::piecewise_linear_distribution<float> calculateDistPWF(
    monsterCatVisualizer::primitives::MinMax<float> radius,
    T boundaries, const T& weights)
{
    const float range = radius.max - radius.min;

    const auto preEnd = std::prev(boundaries.end());
    
    for (auto it = boundaries.begin(); it != preEnd; ++it)
        *it = radius.min + range * (*it);

    *preEnd = radius.max;

    return std::piecewise_linear_distribution<float>(
        boundaries.begin(), boundaries.end(), weights.begin());
}

} // namespace

namespace monsterCatVisualizer::animation
{

ParticlesAnimator::GenerationDescriptor::GenerationDescriptor(
    const SourceGenerationDescriptor& source)
    : radius(calculateDistPWF(source.radius, boundaries, weights))
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
    std::int32_t viewportHeight,
    std::size_t desiredCount)
{
    const float minRadius = utils::SpatialMathUtils::convertSSToNSS(
        DataModel::minRadiusPx, viewportWidth, viewportHeight);

    removeOutsideViewport(timeSeconds, viewportWidth, viewportHeight, desiredCount, appendLimit,
        [&minRadius](const DataModel& particle)
        {
            return std::max(particle.geometry.radius, minRadius);
        });
}

void ParticlesAnimator::appendElements(double timeSeconds,
    std::int32_t viewportWidth,
    std::int32_t viewportHeight,
    std::size_t desiredCount)
{
    appendGeneratedElements(timeSeconds, viewportWidth, viewportHeight, desiredCount, appendLimit);
}

ParticlesAnimator::DataModel ParticlesAnimator::generateElement(double timeSeconds,
    std::int32_t /* viewportWidth */,
    std::int32_t /* viewportHeight */)
{
    const primitives::Vector2<> direction = generateDirection();
    const primitives::ColorRGBA<> color = generateColor();

    return DataModel
    {
        generateTrajectory(timeSeconds, direction),
        generateLifetime(),
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

// Based on particles.js from https://github.com/kaischallenberg/monstercatVisualizer
// Copyright (c) 2025 Kai Schallenberg
// Copyright (c) 2026 Dzmitry Minko
// SPDX-License-Identifier: MIT

#include "animators/particlesAnimator.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <vector>

#include "utils/approximate.h"
#include "utils/mathUtils.h"

namespace monsterCatVisualizer
{

ParticlesAnimator::GenerationDescriptor::GenerationDescriptor(
    const ParticleGenerationDescriptor& particleGenerationDescriptor)
{
    if (!particleGenerationDescriptor.isValid())
    {
        throw std::runtime_error(
            "GenerationDescriptor: failed to convert from invalid descriptor");
    }

    elementsDensity = particleGenerationDescriptor.elementsDensity;

    speed = DistF
    {
        particleGenerationDescriptor.speed.min,
        particleGenerationDescriptor.speed.max
    };

    radius = DistF
    {
        particleGenerationDescriptor.radius.min,
        particleGenerationDescriptor.radius.max,
    };

    colorR = DistU16
    {
        particleGenerationDescriptor.color.min.r,
        particleGenerationDescriptor.color.max.r
    };
    colorG = DistU16
    {
        particleGenerationDescriptor.color.min.g,
        particleGenerationDescriptor.color.max.g
    };
    colorB = DistU16
    {
        particleGenerationDescriptor.color.min.b,
        particleGenerationDescriptor.color.max.b
    };
    colorA = DistF
    {
        particleGenerationDescriptor.color.min.a,
        particleGenerationDescriptor.color.max.a
    };
}


ParticlesAnimator::ParticlesAnimator(StoragePtr storagePtr,
    const ParticleGenerationDescriptor& particleGenerationDescriptor)
    : m_storagePtr(storagePtr),
    m_generationDescriptor(particleGenerationDescriptor)
{
    if (!m_storagePtr)
    {
        throw std::runtime_error("ParticlesAnimator: empty storage");
    }
}

ParticlesAnimator::StoragePtr ParticlesAnimator::getStorage() const
{
    return m_storagePtr;
}

void ParticlesAnimator::initialize(std::size_t desiredCount)
{
    m_storagePtr->reserve(desiredCount);
}

void ParticlesAnimator::clear()
{
    m_storagePtr->clear();
}

void ParticlesAnimator::update(double timeSeconds,
    float viewportWidth,
    float viewportHeight)
{
    if (m_storagePtr->hasPatch())
        return;
    BaseAnimator::update(timeSeconds, viewportWidth, viewportHeight);
}

std::size_t ParticlesAnimator::calculateDesiredCount(float viewportWidth, float viewportHeight) const
{
    const float ratio = utils::SpatialMathUtils::viewportToRatio(viewportWidth, viewportHeight);
    const float viewportMultiplier = std::max(ratio, 1.0f / ratio);
    const float count = std::round(
        m_generationDescriptor.elementsDensity * viewportMultiplier);

    if (!std::isfinite(count) ||
        count >= static_cast<float>(std::numeric_limits<std::size_t>::max()))
        throw std::overflow_error("ParticlesAnimator: desired particle count is too large");

    return static_cast<std::size_t>(count);
}

void ParticlesAnimator::removeElements(double timeSeconds,
    float viewportWidth,
    float viewportHeight)
{
    m_storagePtr->removeIf([=](const Particle& particle)
    {
        const auto position = particle.trajectory.calculatePosition(
            timeSeconds, viewportWidth, viewportHeight);
        const float radius = particle.geometry.radius;

        return utils::Approximate::lessZero(position.x + radius) ||
            utils::Approximate::greater(position.x - radius, 1.0f) ||
            utils::Approximate::lessZero(position.y + radius) ||
            utils::Approximate::greater(position.y - radius, 1.0f);
    });
}

void ParticlesAnimator::appendElements(double timeSeconds, std::size_t desiredCount)
{
    const std::size_t oldCount = m_storagePtr->getDataSize();
    if (desiredCount <= oldCount)
        return;

    const std::size_t newParticlesCount = std::min(desiredCount - oldCount, appendLimit);
    std::vector<Particle> newParticles;
    newParticles.reserve(newParticlesCount);
    for (std::size_t i = newParticlesCount; i > 0; --i)
        newParticles.push_back(generateParticle(timeSeconds));

    m_storagePtr->append(newParticles);
}

Particle ParticlesAnimator::generateParticle(double startTimeSeconds)
{
    const float angle = m_generationDescriptor.angle(m_rng);
    const Vector2<> direction{std::cos(angle), std::sin(angle)};
    const auto color = ColorRGBA<>
        {
            static_cast<std::uint8_t>(m_generationDescriptor.colorR(m_rng)),
            static_cast<std::uint8_t>(m_generationDescriptor.colorG(m_rng)),
            static_cast<std::uint8_t>(m_generationDescriptor.colorB(m_rng)),
            m_generationDescriptor.colorA(m_rng)
        };

    return Particle
    {
        TrajectoryDescriptor
        {
            Vector2<>
            {
                m_generationDescriptor.position(m_rng),
                m_generationDescriptor.position(m_rng)
            },
            direction,
            m_generationDescriptor.speed(m_rng),
            startTimeSeconds
        },
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

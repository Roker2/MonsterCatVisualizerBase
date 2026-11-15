// Based on particles.js from https://github.com/kaischallenberg/monstercatVisualizer
// Copyright (c) 2025 Kai Schallenberg
// Copyright (c) 2026 Dzmitry Minko
// SPDX-License-Identifier: MIT

#include "animators/cloudsAnimator.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <vector>

#include "utils/approximate.h"
#include "utils/mathUtils.h"

namespace monsterCatVisualizer
{

CloudsAnimator::GenerationDescriptor::GenerationDescriptor(
    const CloudGenerationDescriptor& cloudGenerationDescriptor)
{
    if (!cloudGenerationDescriptor.isValid())
    {
        throw std::runtime_error(
            "GenerationDescriptor: failed to convert from invalid descriptor");
    }

    elementsDensity = cloudGenerationDescriptor.elementsDensity;

    speed = DistF
    {
        cloudGenerationDescriptor.speed.min,
        cloudGenerationDescriptor.speed.max
    };

    radius = DistF
    {
        cloudGenerationDescriptor.radius.min,
        cloudGenerationDescriptor.radius.max,
    };

    colorR = DistU16
    {
        cloudGenerationDescriptor.color.min.r,
        cloudGenerationDescriptor.color.max.r
    };
    colorG = DistU16
    {
        cloudGenerationDescriptor.color.min.g,
        cloudGenerationDescriptor.color.max.g
    };
    colorB = DistU16
    {
        cloudGenerationDescriptor.color.min.b,
        cloudGenerationDescriptor.color.max.b
    };
    colorA = DistF
    {
        cloudGenerationDescriptor.color.min.a,
        cloudGenerationDescriptor.color.max.a
    };

    themeColorProbability = cloudGenerationDescriptor.themeColorProbability;
}


CloudsAnimator::CloudsAnimator(StoragePtr storagePtr,
    ColorRGB<> themeColor,
    const CloudGenerationDescriptor& cloudGenerationDescriptor)
    : m_storagePtr(storagePtr),
    m_themeColor(packColor(themeColor.getNormalized())),
    m_generationDescriptor(cloudGenerationDescriptor)
{
    if (!m_storagePtr)
    {
        throw std::runtime_error("CloudsAnimator: empty storage");
    }
}

CloudsAnimator::StoragePtr CloudsAnimator::getStorage() const
{
    return m_storagePtr;
}

ColorRGB<> CloudsAnimator::getThemeColor() const
{
    return unpackColor(m_themeColor.load(std::memory_order_relaxed));
}

void CloudsAnimator::setThemeColor(ColorRGB<> themeColor)
{
    m_themeColor.store(packColor(themeColor.getNormalized()), std::memory_order_relaxed);
}

void CloudsAnimator::initialize(std::size_t desiredCount)
{
    m_storagePtr->reserve(desiredCount);
}

void CloudsAnimator::clear()
{
    m_storagePtr->clear();
}

void CloudsAnimator::update(double timeSeconds,
    float viewportWidth,
    float viewportHeight)
{
    if (m_storagePtr->hasPatch())
        return;
    BaseAnimator::update(timeSeconds, viewportWidth, viewportHeight);
}

std::size_t CloudsAnimator::calculateDesiredCount(float viewportWidth, float viewportHeight) const
{
    const float ratio = utils::SpatialMathUtils::viewportToRatio(viewportWidth, viewportHeight);
    const float viewportMultiplier = std::max(ratio, 1.0f / ratio);
    const float count = std::round(
        m_generationDescriptor.elementsDensity * viewportMultiplier);

    if (!std::isfinite(count) ||
        count >= static_cast<float>(std::numeric_limits<std::size_t>::max()))
        throw std::overflow_error("CloudsAnimator: desired cloud count is too large");

    return static_cast<std::size_t>(count);
}

void CloudsAnimator::removeElements(double timeSeconds,
    float viewportWidth,
    float viewportHeight)
{
    m_storagePtr->removeIf([=](const Cloud& cloud)
    {
        const auto position = cloud.trajectory.calculatePosition(
            timeSeconds, viewportWidth, viewportHeight);
        const float radius = cloud.geometry.radius;

        return utils::Approximate::lessZero(position.x + radius) ||
            utils::Approximate::greater(position.x - radius, 1.0f) ||
            utils::Approximate::lessZero(position.y + radius) ||
            utils::Approximate::greater(position.y - radius, 1.0f);
    });
}

void CloudsAnimator::appendElements(double timeSeconds, std::size_t desiredCount)
{
    const std::size_t oldCount = m_storagePtr->getDataSize();
    if (desiredCount <= oldCount)
        return;

    const std::size_t newCloudsCount = std::min(desiredCount - oldCount, appendLimit);
    std::vector<Cloud> newClouds;
    newClouds.reserve(newCloudsCount);
    for (std::size_t i = newCloudsCount; i > 0; --i)
        newClouds.push_back(generateCloud(timeSeconds));

    m_storagePtr->append(newClouds);
}

Cloud CloudsAnimator::generateCloud(double startTimeSeconds)
{
    const float angle = m_generationDescriptor.angle(m_rng);
    const Vector2<> direction{std::cos(angle), std::sin(angle)};
    const bool isThemeColored = m_generationDescriptor.themeColor(m_rng) <
        m_generationDescriptor.themeColorProbability;
    const auto color = isThemeColored ? ColorRGBA<>{}
        : ColorRGBA<>
        {
            static_cast<std::uint8_t>(m_generationDescriptor.colorR(m_rng)),
            static_cast<std::uint8_t>(m_generationDescriptor.colorG(m_rng)),
            static_cast<std::uint8_t>(m_generationDescriptor.colorB(m_rng)),
            m_generationDescriptor.colorA(m_rng)
        };

    return Cloud
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
            color,
            isThemeColored
        }
    };
}

} // namespace monsterCatVisualizer

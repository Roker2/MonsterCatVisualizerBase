// Based on particles.js from https://github.com/kaischallenberg/monstercatVisualizer
// Copyright (c) 2025 Kai Schallenberg
// Copyright (c) 2026 Dzmitry Minko
// SPDX-License-Identifier: MIT

#include "animators/cloudsAnimator.h"

namespace monsterCatVisualizer::animators
{

CloudsAnimator::GenerationDescriptor::GenerationDescriptor(
    const CloudGenerationDescriptor& source)
    : radius(source.radius.min, source.radius.max),
      themeColorProbability(source.themeColorProbability)
{
}

CloudsAnimator::CloudsAnimator(primitives::ColorRGB<> themeColor,
    StoragePtr storagePtr,
    const CloudGenerationDescriptor& cloudGenerationDescriptor)
    : ContainerAnimator<Cloud>(std::move(storagePtr), cloudGenerationDescriptor),
      m_themeColor(packColor(themeColor.getNormalized())),
      m_generationDescriptor(cloudGenerationDescriptor)
{
}

primitives::ColorRGB<> CloudsAnimator::getThemeColor() const
{
    return primitives::unpackColor(m_themeColor.load(std::memory_order_relaxed));
}

void CloudsAnimator::setThemeColor(primitives::ColorRGB<> themeColor)
{
    m_themeColor.store(primitives::packColor(themeColor.getNormalized()), std::memory_order_relaxed);
}

void CloudsAnimator::removeElements(double timeSeconds,
    float viewportWidth,
    float viewportHeight)
{
    removeOutsideViewport(timeSeconds, viewportWidth, viewportHeight,
        [](const Cloud& cloud) { return cloud.geometry.radius; });
}

void CloudsAnimator::appendElements(double timeSeconds, std::size_t desiredCount)
{
    appendGeneratedElements(timeSeconds, desiredCount, appendLimit);
}

CloudsAnimator::Cloud CloudsAnimator::generateElement(double startTimeSeconds)
{
    const primitives::Vector2<> direction = generateDirection();
    const bool isThemeColored = m_generationDescriptor.themeColor(m_rng) <
        m_generationDescriptor.themeColorProbability;

    primitives::ColorRGBA<> color{};
    if (isThemeColored)
        color.a = m_sharedGenerationDescriptor.colorA(m_rng);
    else
        color = generateColor();

    return Cloud
    {
        generateTrajectory(startTimeSeconds, direction),
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

} // namespace monsterCatVisualizer::animators

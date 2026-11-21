// Based on particles.js from https://github.com/kaischallenberg/monstercatVisualizer
// Copyright (c) 2025 Kai Schallenberg
// Copyright (c) 2026 Dzmitry Minko
// SPDX-License-Identifier: MIT

#include "animation/cloudsAnimator.h"

namespace monsterCatVisualizer::animation
{

CloudsAnimator::GenerationDescriptor::GenerationDescriptor(
    const SourceGenerationDescriptor& source)
    : radius(source.radius.min, source.radius.max),
      themeColorProbability(source.themeColorProbability)
{
}

CloudsAnimator::CloudsAnimator(primitives::ColorRGB<> themeColor,
    StoragePtr storagePtr,
    const SourceGenerationDescriptor& source)
    : ContainerAnimator<DataModel>(std::move(storagePtr), source),
      m_themeColor(packColor(themeColor.getNormalized())),
      m_generationDescriptor(source)
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
    std::int32_t viewportWidth,
    std::int32_t viewportHeight)
{
    removeOutsideViewport(timeSeconds, viewportWidth, viewportHeight,
        [](const DataModel& cloud) { return cloud.geometry.radius; });
}

void CloudsAnimator::appendElements(double timeSeconds, std::size_t desiredCount)
{
    appendGeneratedElements(timeSeconds, desiredCount, appendLimit);
}

CloudsAnimator::DataModel CloudsAnimator::generateElement(double startTimeSeconds)
{
    const primitives::Vector2<> direction = generateDirection();
    const bool isThemeColored = m_generationDescriptor.themeColor(m_rng) <
        m_generationDescriptor.themeColorProbability;

    primitives::ColorRGBA<> color{};
    if (isThemeColored)
        color.a = m_sharedGenerationDescriptor.colorA(m_rng);
    else
        color = generateColor();

    return DataModel
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

} // namespace monsterCatVisualizer::animation

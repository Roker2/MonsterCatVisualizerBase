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

CloudsAnimator::CloudsAnimator(ThemeColorStatePtr themeColorState,
    StoragePtr storagePtr,
    const SourceGenerationDescriptor& source)
    : ContainerAnimator<DataModel>(std::move(storagePtr), source),
      m_themeColorState(std::move(themeColorState)),
      m_generationDescriptor(source)
{
    if (!m_themeColorState)
        throw std::runtime_error("CloudsAnimator: empty themeColorState");
}

CloudsAnimator::ThemeColorStatePtr CloudsAnimator::getThemeColorState() const
{
    return m_themeColorState;
}

void CloudsAnimator::removeElements(double timeSeconds,
    std::int32_t viewportWidth,
    std::int32_t viewportHeight,
    std::size_t desiredCount)
{
    removeOutsideViewport(timeSeconds, viewportWidth, viewportHeight, desiredCount,
        [](const DataModel& cloud) { return cloud.geometry.radius; });
}

void CloudsAnimator::appendElements(double timeSeconds,
    std::int32_t viewportWidth,
    std::int32_t viewportHeight,
    std::size_t desiredCount)
{
    appendGeneratedElements(timeSeconds, viewportWidth, viewportHeight, desiredCount, appendLimit);
}

CloudsAnimator::DataModel CloudsAnimator::generateElement(double timeSeconds,
    std::int32_t /* viewportWidth */,
    std::int32_t /* viewportHeight */)
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
        generateTrajectory(timeSeconds, direction),
        generateLifetime(),
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

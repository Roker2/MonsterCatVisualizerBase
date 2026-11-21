// Based on particles.js from https://github.com/kaischallenberg/monstercatVisualizer
// Copyright (c) 2025 Kai Schallenberg
// Copyright (c) 2026 Dzmitry Minko
// SPDX-License-Identifier: MIT

#include "animation/linesAnimator.h"

namespace monsterCatVisualizer::animation
{

LinesAnimator::GenerationDescriptor::GenerationDescriptor(
    const SourceGenerationDescriptor& source)
    : length(source.length.min, source.length.max),
      lineWidth(source.lineWidth)
{
}

LinesAnimator::LinesAnimator(StoragePtr storagePtr,
    const SourceGenerationDescriptor& source)
    : ContainerAnimator<DataModel>(std::move(storagePtr), source),
      m_generationDescriptor(source)
{
}

void LinesAnimator::removeElements(double timeSeconds,
    std::int32_t viewportWidth,
    std::int32_t viewportHeight)
{
    removeOutsideViewport(timeSeconds, viewportWidth, viewportHeight,
        [](const DataModel& line)
        {
            return line.geometry.length + line.geometry.lineWidth;
        });
}

void LinesAnimator::appendElements(double timeSeconds, std::size_t desiredCount)
{
    appendGeneratedElements(timeSeconds, desiredCount, appendLimit);
}

LinesAnimator::DataModel LinesAnimator::generateElement(double startTimeSeconds)
{
    const primitives::Vector2<> direction = generateDirection();
    const primitives::ColorRGBA<> color = generateColor();

    return DataModel
    {
        generateTrajectory(startTimeSeconds, direction),
        GeometryDescriptor
        {
            m_generationDescriptor.length(m_rng),
            m_generationDescriptor.lineWidth
        },
        VisualDescriptor
        {
            color
        }
    };
}

} // namespace monsterCatVisualizer::animation

// Based on particles.js from https://github.com/kaischallenberg/monstercatVisualizer
// Copyright (c) 2025 Kai Schallenberg
// Copyright (c) 2026 Dzmitry Minko
// SPDX-License-Identifier: MIT

#include "animators/linesAnimator.h"

namespace monsterCatVisualizer::animators
{

LinesAnimator::GenerationDescriptor::GenerationDescriptor(
    const LineGenerationDescriptor& source)
    : length(source.length.min, source.length.max),
      lineWidth(source.lineWidth)
{
}

LinesAnimator::LinesAnimator(StoragePtr storagePtr,
    const LineGenerationDescriptor& lineGenerationDescriptor)
    : ContainerAnimator<Line>(std::move(storagePtr), lineGenerationDescriptor),
      m_generationDescriptor(lineGenerationDescriptor)
{
}

void LinesAnimator::removeElements(double timeSeconds,
    float viewportWidth,
    float viewportHeight)
{
    removeOutsideViewport(timeSeconds, viewportWidth, viewportHeight,
        [](const Line& line)
        {
            return line.geometry.length + line.geometry.lineWidth;
        });
}

void LinesAnimator::appendElements(double timeSeconds, std::size_t desiredCount)
{
    appendGeneratedElements(timeSeconds, desiredCount, appendLimit);
}

LinesAnimator::Line LinesAnimator::generateElement(double startTimeSeconds)
{
    const primitives::Vector2<> direction = generateDirection();
    const primitives::ColorRGBA<> color = generateColor();

    return Line
    {
        generateTrajectory(startTimeSeconds, direction),
        GeometryDescriptor
        {
            direction,
            m_generationDescriptor.length(m_rng),
            m_generationDescriptor.lineWidth
        },
        VisualDescriptor
        {
            color
        }
    };
}

} // namespace monsterCatVisualizer::animators

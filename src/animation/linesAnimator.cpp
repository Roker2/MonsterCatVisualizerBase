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

LinesAnimator::LinesAnimator(const SourceGenerationDescriptor& source,
    StoragePtr storagePtr)
    : ContainerAnimator<DataModel>(source, std::move(storagePtr)),
      m_generationDescriptor(source)
{
}

void LinesAnimator::update(double timeSeconds,
    std::int32_t viewportWidth,
    std::int32_t viewportHeight)
{
    if (m_storagePtr->hasPatch())
        return;
    const std::size_t desiredCount = calculateDesiredCount(viewportWidth, viewportHeight);
    removeOutsideViewport(timeSeconds, viewportWidth, viewportHeight, desiredCount,
        [](const DataModel& line)
        {
            return (line.geometry.length + line.geometry.lineWidth) * 0.5f;
        });
    if (appendGeneratedElements(timeSeconds, viewportWidth, viewportHeight, desiredCount, appendLimit))
    {
        m_generateInitial = false;
    }
}

LinesAnimator::DataModel LinesAnimator::generateElement(double timeSeconds,
    std::int32_t viewportWidth,
    std::int32_t viewportHeight)
{
    const primitives::Vector2<> direction = generateDirection();
    const primitives::ColorRGBA<> color = generateColor();
    const float length = m_generationDescriptor.length(m_rng);

    const auto trajectory = m_generateInitial
        ? generateTrajectory(timeSeconds, direction)
        : generateCustomTrajectory(timeSeconds, viewportWidth, viewportHeight, direction, length);

    return DataModel
    {
        trajectory,
        generateLifetime(),
        GeometryDescriptor
        {
            length,
            m_generationDescriptor.lineWidth
        },
        VisualDescriptor
        {
            color
        }
    };
}

models::TrajectoryDescriptor LinesAnimator::generateCustomTrajectory(double timeSeconds,
    std::int32_t viewportWidth,
    std::int32_t viewportHeight,
    primitives::Vector2<> direction,
    float length)
{
    const bool isDirectionHorizontal =
        std::abs(direction.x) >= std::abs(direction.y);

    const auto halfLineOffsetNVS = utils::SpatialMathUtils::convertNSSToNVS(
        direction * (length * 0.5f), viewportWidth, viewportHeight);

    const float mainDimensionOffset = isDirectionHorizontal
        ? halfLineOffsetNVS.x : halfLineOffsetNVS.y;

    const float otherDimensionPosition = m_generationDescriptor.scalarPosition(m_rng);
    const float mainDimensionPosition = std::signbit(mainDimensionOffset)
        ? 1.0f - mainDimensionOffset
        : -mainDimensionOffset;

    const primitives::Vector2<> startPosition
    {
        isDirectionHorizontal ? mainDimensionPosition : otherDimensionPosition,
        isDirectionHorizontal ? otherDimensionPosition : mainDimensionPosition
    };

    return models::TrajectoryDescriptor
    {
        startPosition,
        direction,
        m_sharedGenerationDescriptor.speed(m_rng),
        timeSeconds
    };
}

} // namespace monsterCatVisualizer::animation

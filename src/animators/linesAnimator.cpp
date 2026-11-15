// Based on particles.js from https://github.com/kaischallenberg/monstercatVisualizer
// Copyright (c) 2025 Kai Schallenberg
// Copyright (c) 2026 Dzmitry Minko
// SPDX-License-Identifier: MIT

#include "animators/linesAnimator.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <vector>

#include "utils/approximate.h"
#include "utils/mathUtils.h"

namespace monsterCatVisualizer
{

LinesAnimator::GenerationDescriptor::GenerationDescriptor(
    const LineGenerationDescriptor& lineGenerationDescriptor)
{
    if (!lineGenerationDescriptor.isValid())
    {
        throw std::runtime_error(
            "GenerationDescriptor: failed to convert from invalid descriptor");
    }

    elementsDensity = lineGenerationDescriptor.elementsDensity;

    speed = DistF
    {
        lineGenerationDescriptor.speed.min,
        lineGenerationDescriptor.speed.max
    };

    length = DistF
    {
        lineGenerationDescriptor.length.min,
        lineGenerationDescriptor.length.max,
    };
    lineWidth = lineGenerationDescriptor.lineWidth;

    colorR = DistU16
    {
        lineGenerationDescriptor.color.min.r,
        lineGenerationDescriptor.color.max.r
    };
    colorG = DistU16
    {
        lineGenerationDescriptor.color.min.g,
        lineGenerationDescriptor.color.max.g
    };
    colorB = DistU16
    {
        lineGenerationDescriptor.color.min.b,
        lineGenerationDescriptor.color.max.b
    };
    colorA = DistF
    {
        lineGenerationDescriptor.color.min.a,
        lineGenerationDescriptor.color.max.a
    };
}


LinesAnimator::LinesAnimator(StoragePtr storagePtr,
    const LineGenerationDescriptor& lineGenerationDescriptor)
    : m_storagePtr(storagePtr),
    m_generationDescriptor(lineGenerationDescriptor)
{
    if (!m_storagePtr)
    {
        throw std::runtime_error("LinesAnimator: empty storage");
    }
}

LinesAnimator::StoragePtr LinesAnimator::getStorage() const
{
    return m_storagePtr;
}

void LinesAnimator::initialize(std::size_t desiredCount)
{
    m_storagePtr->reserve(desiredCount);
}

void LinesAnimator::clear()
{
    m_storagePtr->clear();
}

void LinesAnimator::update(double timeSeconds,
    float viewportWidth,
    float viewportHeight)
{
    if (m_storagePtr->hasPatch())
        return;
    BaseAnimator::update(timeSeconds, viewportWidth, viewportHeight);
}

std::size_t LinesAnimator::calculateDesiredCount(float viewportWidth, float viewportHeight) const
{
    const float ratio = utils::SpatialMathUtils::viewportToRatio(viewportWidth, viewportHeight);
    const float viewportMultiplier = std::max(ratio, 1.0f / ratio);
    const float count = std::round(
        m_generationDescriptor.elementsDensity * viewportMultiplier);

    if (!std::isfinite(count) ||
        count >= static_cast<float>(std::numeric_limits<std::size_t>::max()))
        throw std::overflow_error("LinesAnimator: desired line count is too large");

    return static_cast<std::size_t>(count);
}

void LinesAnimator::removeElements(double timeSeconds,
    float viewportWidth,
    float viewportHeight)
{
    m_storagePtr->removeIf([=](const Line& line)
    {
        const auto position = line.trajectory.calculatePosition(
            timeSeconds, viewportWidth, viewportHeight);
        const float extent = line.geometry.length + line.geometry.lineWidth;

        return utils::Approximate::lessZero(position.x + extent) ||
            utils::Approximate::greater(position.x - extent, 1.0f) ||
            utils::Approximate::lessZero(position.y + extent) ||
            utils::Approximate::greater(position.y - extent, 1.0f);
    });
}

void LinesAnimator::appendElements(double timeSeconds, std::size_t desiredCount)
{
    const std::size_t oldCount = m_storagePtr->getDataSize();
    if (desiredCount <= oldCount)
        return;

    const std::size_t newLinesCount = std::min(desiredCount - oldCount, appendLimit);
    std::vector<Line> newLines;
    newLines.reserve(newLinesCount);
    for (std::size_t i = newLinesCount; i > 0; --i)
        newLines.push_back(generateLine(timeSeconds));

    m_storagePtr->append(newLines);
}

Line LinesAnimator::generateLine(double startTimeSeconds)
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

    return Line
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

} // namespace monsterCatVisualizer

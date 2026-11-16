// Copyright (c) 2026 Dzmitry Minko
// SPDX-License-Identifier: MIT

#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <random>
#include <stdexcept>
#include <utility>
#include <vector>

#include "animators/baseAnimator.h"
#include "primitives/color.h"
#include "primitives/vector2.h"
#include "models/container.h"
#include "models/trajectory.h"
#include "utils/approximate.h"
#include "utils/defines.h"
#include "utils/mathUtils.h"

namespace monsterCatVisualizer::animators
{

template <typename T>
class ContainerAnimator : public BaseAnimator
{
public:
    using Storage = models::Container<T>;
    using StoragePtr = std::shared_ptr<Storage>;

    StoragePtr getStorage() const
    {
        return m_storagePtr;
    }

    using BaseAnimator::initialize;

    void initialize(std::size_t desiredCount) override
    {
        m_storagePtr->reserve(desiredCount);
    }

    void clear() override
    {
        m_storagePtr->clear();
    }

    // An update is not atomic across the patch check, removal, and append.
    // Intended for one producer and one consumer; no other thread may mutate
    // this storage while the producer is updating it.
    void update(double timeSeconds, float viewportWidth, float viewportHeight) override
    {
        if (m_storagePtr->hasPatch())
            return;
        BaseAnimator::update(timeSeconds, viewportWidth, viewportHeight);
    }

    std::size_t calculateDesiredCount(float viewportWidth, float viewportHeight) const override
    {
        const float ratio = utils::SpatialMathUtils::viewportToRatio(viewportWidth, viewportHeight);
        const float viewportMultiplier = std::max(ratio, 1.0f / ratio);
        const float count = std::round(m_sharedGenerationDescriptor.elementsDensity * viewportMultiplier);

        if (!std::isfinite(count) ||
            count >= static_cast<float>(std::numeric_limits<std::size_t>::max()))
            throw std::overflow_error("ContainerAnimator: desired element count is too large");

        return static_cast<std::size_t>(count);
    }

protected:
    struct SharedGenerationDescriptor
    {
        using DistF = std::uniform_real_distribution<float>;
        using DistU16 = std::uniform_int_distribution<std::uint16_t>;

        template <typename SourceDescriptor>
        explicit SharedGenerationDescriptor(const SourceDescriptor& source)
        {
            if (!source.isValid())
                throw std::runtime_error("GenerationDescriptor: failed to convert from invalid descriptor");

            elementsDensity = source.elementsDensity;
            speed = DistF{source.speed.min, source.speed.max};
            colorR = DistU16{source.color.min.r, source.color.max.r};
            colorG = DistU16{source.color.min.g, source.color.max.g};
            colorB = DistU16{source.color.min.b, source.color.max.b};
            colorA = DistF{source.color.min.a, source.color.max.a};
        }

        float elementsDensity{};

        DistF position{0.0f, 1.0f};
        DistF angle{0.0f, utils::twoPi};
        DistF speed{};

        DistU16 colorR{};
        DistU16 colorG{};
        DistU16 colorB{};
        DistF colorA{};
    };

    template <typename SourceDescriptor>
    ContainerAnimator(StoragePtr storagePtr, const SourceDescriptor& source)
        : m_storagePtr(std::move(storagePtr)),
          m_sharedGenerationDescriptor(source)
    {
        if (!m_storagePtr)
            throw std::runtime_error("ContainerAnimator: empty storage");
    }

    template <typename Extent>
    void removeOutsideViewport(double timeSeconds, float viewportWidth,
        float viewportHeight, Extent&& extent)
    {
        m_storagePtr->removeIf([&](const T& element)
        {
            const auto position = element.trajectory.calculatePosition(
                timeSeconds, viewportWidth, viewportHeight);
            const float r = extent(element);

            return utils::Approximate::lessZero(position.x + r) ||
                utils::Approximate::greater(position.x - r, 1.0f) ||
                utils::Approximate::lessZero(position.y + r) ||
                utils::Approximate::greater(position.y - r, 1.0f);
        });
    }

    void appendGeneratedElements(double timeSeconds, std::size_t desiredCount,
        std::size_t appendLimit)
    {
        const std::size_t oldCount = m_storagePtr->getDataSize();
        if (desiredCount <= oldCount)
            return;

        const std::size_t newCount = std::min(desiredCount - oldCount, appendLimit);
        std::vector<T> newElements;
        newElements.reserve(newCount);
        for (std::size_t i = newCount; i > 0; --i)
            newElements.push_back(generateElement(timeSeconds));

        m_storagePtr->append(newElements);
    }

    virtual T generateElement(double startTimeSeconds) = 0;

    models::TrajectoryDescriptor generateTrajectory(double startTimeSeconds, primitives::Vector2<> direction)
    {
        return models::TrajectoryDescriptor
        {
            primitives::Vector2<>
            {
                m_sharedGenerationDescriptor.position(m_rng),
                m_sharedGenerationDescriptor.position(m_rng)
            },
            direction,
            m_sharedGenerationDescriptor.speed(m_rng),
            startTimeSeconds
        };
    }

    primitives::Vector2<> generateDirection()
    {
        const float angle = m_sharedGenerationDescriptor.angle(m_rng);
        return primitives::Vector2<>{std::cos(angle), std::sin(angle)};
    }

    primitives::ColorRGBA<> generateColor()
    {
        return primitives::ColorRGBA<>
        {
            static_cast<std::uint8_t>(m_sharedGenerationDescriptor.colorR(m_rng)),
            static_cast<std::uint8_t>(m_sharedGenerationDescriptor.colorG(m_rng)),
            static_cast<std::uint8_t>(m_sharedGenerationDescriptor.colorB(m_rng)),
            m_sharedGenerationDescriptor.colorA(m_rng)
        };
    }

    StoragePtr m_storagePtr;
    SharedGenerationDescriptor m_sharedGenerationDescriptor;
};

} // namespace monsterCatVisualizer::animators

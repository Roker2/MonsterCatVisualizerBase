// Copyright (c) 2026 Dzmitry Minko
// SPDX-License-Identifier: MIT

#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <random>
#include <stdexcept>
#include <utility>
#include <vector>

#include "utils/approximate.h"
#include "utils/defines.h"
#include "utils/mathUtils.h"
#include "primitives/color.h"
#include "primitives/vector2.h"
#include "models/container.h"
#include "models/lifetime.h"
#include "models/trajectory.h"
#include "animation/iAnimator.h"

namespace monsterCatVisualizer::animation
{

template <typename T>
class ContainerAnimator : public IAnimator
{
public:
    using DataModel = T;
    using SourceGenerationDescriptor = typename DataModel::GenerationDescriptor;
    using GeometryDescriptor = typename DataModel::GeometryDescriptor;
    using VisualDescriptor = typename DataModel::VisualDescriptor;

    using Storage = models::Container<DataModel>;
    using StoragePtr = std::shared_ptr<Storage>;

    StoragePtr getStorage() const
    {
        return m_storagePtr;
    }

    void initialize(std::int32_t viewportWidth,
        std::int32_t viewportHeight) override
    {
        const std::size_t desiredCount = calculateDesiredCount(viewportWidth, viewportHeight);
        m_storagePtr->reserve(desiredCount);
    }

    void clear() override
    {
        m_storagePtr->clear();
    }

protected:
    struct SharedGenerationDescriptor
    {
        using DistF = std::uniform_real_distribution<float>;
        using DistD = std::uniform_real_distribution<double>;
        using DistU16 = std::uniform_int_distribution<std::uint16_t>;

        template <typename SourceDescriptor>
        explicit SharedGenerationDescriptor(const SourceDescriptor& source)
        {
            if (!source.isValid())
                throw std::runtime_error("GenerationDescriptor: failed to convert from invalid descriptor");

            elementsDensity = source.elementsDensity;
            speed = DistF{source.speed.min, source.speed.max};
            lifetimeSeconds = DistD{source.lifetimeSeconds.min, source.lifetimeSeconds.max};
            fadeInSeconds = DistD{source.fadeInSeconds.min, source.fadeInSeconds.max};
            fadeOutSeconds = DistD{source.fadeOutSeconds.min, source.fadeOutSeconds.max};

            colorR = DistU16{source.color.min.r, source.color.max.r};
            colorG = DistU16{source.color.min.g, source.color.max.g};
            colorB = DistU16{source.color.min.b, source.color.max.b};
            colorA = DistF{source.color.min.a, source.color.max.a};
        }

        float elementsDensity{};

        DistF position{0.0f, 1.0f};
        DistF angle{0.0f, utils::twoPi};
        DistF speed{};

        DistD lifetimeSeconds{};
        DistD fadeInSeconds{};
        DistD fadeOutSeconds{};

        DistU16 colorR{};
        DistU16 colorG{};
        DistU16 colorB{};
        DistF colorA{};

        float skipAppendChance{0.4f};
        DistF skipAppend{0.0f, 1.0f};
        DistF appendElementsMultiplier{0.6f, 0.9f};
    };

    template <typename SourceDescriptor>
    ContainerAnimator(const SourceDescriptor& source,
        StoragePtr storagePtr)
        : m_sharedGenerationDescriptor(source),
        m_storagePtr(std::move(storagePtr))
    {
        if (!m_storagePtr)
            throw std::runtime_error("ContainerAnimator: empty storage");
    }

    template <typename Extent>
    void removeOutsideViewport(double timeSeconds,
        std::int32_t viewportWidth,
        std::int32_t viewportHeight,
        std::size_t /* desiredCount */,
        Extent&& extent)
    {
        m_storagePtr->removeIf([&](const DataModel& element)
        {
            const double age = timeSeconds - element.trajectory.startTimeSeconds;
            if (utils::Approximate::greater(age, element.lifetime.lifetimeSeconds))
                return true;
            if (utils::Approximate::less(age, element.lifetime.fadeInSeconds))
                return false;

            const auto position = element.trajectory.calculatePosition(
                timeSeconds, viewportWidth, viewportHeight);
            const float flatExtentNSS = extent(element);
            const auto extentNVS = utils::SpatialMathUtils::convertNSSToNVS(
                primitives::Vector2<>{flatExtentNSS, flatExtentNSS}, viewportWidth, viewportHeight);

            return utils::Approximate::lessZero(position.x + extentNVS.x) ||
                utils::Approximate::greater(position.x - extentNVS.x, 1.0f) ||
                utils::Approximate::lessZero(position.y + extentNVS.y) ||
                utils::Approximate::greater(position.y - extentNVS.y, 1.0f);
        });
    }

    /**
     * Append new elements.
     * @return true if append happend (was not skipped)
     */
    bool appendGeneratedElements(double timeSeconds,
        std::int32_t viewportWidth,
        std::int32_t viewportHeight,
        std::size_t desiredCount,
        std::size_t appendLimit)
    {
        const std::size_t oldCount = m_storagePtr->getDataSize();
        if (desiredCount <= oldCount)
            return false;

        if (m_sharedGenerationDescriptor.skipAppend(m_rng) <
            m_sharedGenerationDescriptor.skipAppendChance)
            return false;

        const std::size_t newRawCount = std::min(desiredCount - oldCount, appendLimit);
        const float appendElementsMultiplier =
            m_sharedGenerationDescriptor.appendElementsMultiplier(m_rng);
        const std::size_t newCount = std::clamp(
            static_cast<std::size_t>(std::round(newRawCount * appendElementsMultiplier)),
            std::size_t{1},
            newRawCount);

        std::vector<DataModel> newElements;
        newElements.reserve(newCount);
        for (std::size_t i = newCount; i > 0; --i)
            newElements.push_back(generateElement(
                timeSeconds, viewportWidth, viewportHeight));

        m_storagePtr->append(newElements);

        return true;
    }

    virtual DataModel generateElement(double timeSeconds,
        std::int32_t viewportWidth,
        std::int32_t viewportHeight) = 0;

    std::size_t calculateDesiredCount(std::int32_t viewportWidth,
        std::int32_t viewportHeight) const
    {
        return utils::SpatialMathUtils::calculateDesiredCount(
            m_sharedGenerationDescriptor.elementsDensity, viewportWidth, viewportHeight);
    }

    models::TrajectoryDescriptor generateTrajectory(double timeSeconds, primitives::Vector2<> direction)
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
            timeSeconds
        };
    }

    models::LifetimeDescriptor generateLifetime()
    {
        return models::LifetimeDescriptor
        {
            m_sharedGenerationDescriptor.lifetimeSeconds(m_rng),
            m_sharedGenerationDescriptor.fadeInSeconds(m_rng),
            m_sharedGenerationDescriptor.fadeOutSeconds(m_rng),
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

    SharedGenerationDescriptor m_sharedGenerationDescriptor;
    StoragePtr m_storagePtr;
    std::mt19937 m_rng{std::random_device{}()};
};

} // namespace monsterCatVisualizer::animation

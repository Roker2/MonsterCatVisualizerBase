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
#include "animation/baseAnimator.h"

namespace monsterCatVisualizer::animation
{

template <typename T>
class ContainerAnimator : public BaseAnimator
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

    using BaseAnimator::initialize;

    void initialize(std::size_t desiredCount) override
    {
        m_storagePtr->reserve(desiredCount);
    }

    void clear() override
    {
        m_storagePtr->clear();
    }

    /**
     * An update is not atomic across the patch check, removal, and append.
     * Intended for one producer and one consumer.
     * No other thread may mutate this storage while the producer is updating it.
     */
    void update(double timeSeconds, std::int32_t viewportWidth, std::int32_t viewportHeight) override
    {
        if (m_storagePtr->hasPatch())
            return;
        BaseAnimator::update(timeSeconds, viewportWidth, viewportHeight);
    }

    std::size_t calculateDesiredCount(std::int32_t viewportWidth, std::int32_t viewportHeight) const override
    {
        return utils::SpatialMathUtils::calculateDesiredCount(
            m_sharedGenerationDescriptor.elementsDensity, viewportWidth, viewportHeight);
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
    void removeOutsideViewport(double timeSeconds, std::int32_t viewportWidth,
        std::int32_t viewportHeight, Extent&& extent)
    {
        m_storagePtr->removeIf([&](const DataModel& element)
        {
            if (utils::Approximate::greater(
                timeSeconds - element.trajectory.startTimeSeconds,
                element.lifetime.lifetimeSeconds))
                return true;

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

    void appendGeneratedElements(double timeSeconds, std::size_t desiredCount,
        std::size_t appendLimit)
    {
        const std::size_t oldCount = m_storagePtr->getDataSize();
        if (desiredCount <= oldCount)
            return;

        const std::size_t newCount = std::min(desiredCount - oldCount, appendLimit);
        std::vector<DataModel> newElements;
        newElements.reserve(newCount);
        for (std::size_t i = newCount; i > 0; --i)
            newElements.push_back(generateElement(timeSeconds));

        m_storagePtr->append(newElements);
    }

    virtual DataModel generateElement(double startTimeSeconds) = 0;

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

    StoragePtr m_storagePtr;
    SharedGenerationDescriptor m_sharedGenerationDescriptor;
};

} // namespace monsterCatVisualizer::animation

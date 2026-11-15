// Based on particles.js from https://github.com/kaischallenberg/monstercatVisualizer
// Copyright (c) 2025 Kai Schallenberg
// Copyright (c) 2026 Dzmitry Minko
// SPDX-License-Identifier: MIT

#pragma once

#include <atomic>
#include <cstdint>
#include <memory>
#include <random>

#include "utils/defines.h"
#include "models/cloud.h"
#include "models/container.h"
#include "animators/baseAnimator.h"

namespace monsterCatVisualizer
{

class CloudsAnimator : public BaseAnimator
{
public:
    static constexpr std::size_t appendLimit = 10;

    struct GenerationDescriptor
    {
        using DistF = std::uniform_real_distribution<float>;
        using DistU16 = std::uniform_int_distribution<std::uint16_t>;

        GenerationDescriptor(const CloudGenerationDescriptor& cloudGenerationDescriptor);

        float elementsDensity{};

        DistF position{0.0f, 1.0f};
        DistF angle{0.0f, utils::twoPi};
        DistF speed{};
        DistF radius{};
        DistU16 colorR{};
        DistU16 colorG{};
        DistU16 colorB{};
        DistF colorA{};
        DistF themeColor{0.0, 1.0f};
        float themeColorProbability{};
    };

    using GeometryDescriptor = Cloud::GeometryDescriptor;
    using VisualDescriptor = Cloud::VisualDescriptor;
    using Storage = Container<Cloud>;
    using StoragePtr = std::shared_ptr<Storage>;
    using ThemeColorStorage = std::atomic<std::uint32_t>;

    CloudsAnimator(StoragePtr storagePtr,
        ColorRGB<> themeColor,
        const CloudGenerationDescriptor& cloudGenerationDescriptor);

    StoragePtr getStorage() const;
    ColorRGB<> getThemeColor() const;
    void setThemeColor(ColorRGB<> themeColor);

    using BaseAnimator::initialize;
    void initialize(std::size_t desiredCount) override;
    void clear() override;
    virtual void update(double timeSeconds,
        float viewportWidth,
        float viewportHeight);
    std::size_t calculateDesiredCount(float viewportWidth,
        float viewportHeight) const override;

protected:
    void removeElements(double timeSeconds,
        float viewportWidth,
        float viewportHeight) override;
    void appendElements(double timeSeconds, std::size_t desiredCount) override;

    Cloud generateCloud(double startTimeSeconds);

private:
    static constexpr std::uint32_t packColor(ColorRGB<> color) noexcept
    {
        return (static_cast<std::uint32_t>(color.r) << 16u) |
            (static_cast<std::uint32_t>(color.g) << 8u) |
            static_cast<std::uint32_t>(color.b);
    }

    static constexpr ColorRGB<> unpackColor(std::uint32_t color) noexcept
    {
        return ColorRGB<>
        {
            static_cast<std::uint8_t>((color >> 16u) & 0xffu),
            static_cast<std::uint8_t>((color >> 8u) & 0xffu),
            static_cast<std::uint8_t>(color & 0xffu)
        };
    }

protected:
    StoragePtr m_storagePtr;
    ThemeColorStorage m_themeColor{};
    GenerationDescriptor m_generationDescriptor;
};

} // namespace monsterCatVisualizer
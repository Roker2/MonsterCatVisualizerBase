// Based on particles.js from https://github.com/kaischallenberg/monstercatVisualizer
// Copyright (c) 2025 Kai Schallenberg
// Copyright (c) 2026 Dzmitry Minko
// SPDX-License-Identifier: MIT

#pragma once

#include <atomic>
#include <cstdint>
#include <random>

#include "models/cloud.h"
#include "animation/containerAnimator.h"

namespace monsterCatVisualizer::animation
{

class CloudsAnimator : public ContainerAnimator<models::Cloud>
{
public:
    static constexpr std::size_t appendLimit = 25;

    using ThemeColorStorage = std::atomic<std::uint32_t>;

    struct GenerationDescriptor
    {
        using DistF = std::uniform_real_distribution<float>;

        explicit GenerationDescriptor(const SourceGenerationDescriptor& source);

        DistF radius{};
        DistF themeColor{0.0f, 1.0f};
        float themeColorProbability{};
    };

    CloudsAnimator(primitives::ColorRGB<> themeColor,
        StoragePtr storagePtr,
        const SourceGenerationDescriptor& source);

    primitives::ColorRGB<> getThemeColor() const;
    void setThemeColor(primitives::ColorRGB<> themeColor);

protected:
    void removeElements(double timeSeconds,
        std::int32_t viewportWidth,
        std::int32_t viewportHeight) override;
    void appendElements(double timeSeconds, std::size_t desiredCount) override;

    DataModel generateElement(double startTimeSeconds) override;

    ThemeColorStorage m_themeColor{};
    GenerationDescriptor m_generationDescriptor;
};

} // namespace monsterCatVisualizer::animation

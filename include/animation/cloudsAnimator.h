// Based on particles.js from https://github.com/kaischallenberg/monstercatVisualizer
// Copyright (c) 2025 Kai Schallenberg
// Copyright (c) 2026 Dzmitry Minko
// SPDX-License-Identifier: MIT

#pragma once

#include <atomic>
#include <cstdint>
#include <random>

#include "animation/containerAnimator.h"
#include "models/cloud.h"

namespace monsterCatVisualizer::animation
{

class CloudsAnimator : public ContainerAnimator<models::Cloud>
{
public:
    static constexpr std::size_t appendLimit = 10;

    using CloudGenerationDescriptor = models::CloudGenerationDescriptor;
    using Cloud = models::Cloud;

    struct GenerationDescriptor
    {
        using DistF = std::uniform_real_distribution<float>;

        explicit GenerationDescriptor(const CloudGenerationDescriptor& source);

        DistF radius{};
        DistF themeColor{0.0f, 1.0f};
        float themeColorProbability{};
    };

    using GeometryDescriptor = Cloud::GeometryDescriptor;
    using VisualDescriptor = Cloud::VisualDescriptor;
    using Storage = ContainerAnimator<Cloud>::Storage;
    using StoragePtr = ContainerAnimator<Cloud>::StoragePtr;
    using ThemeColorStorage = std::atomic<std::uint32_t>;

    CloudsAnimator(primitives::ColorRGB<> themeColor,
        StoragePtr storagePtr,
        const CloudGenerationDescriptor& cloudGenerationDescriptor);

    primitives::ColorRGB<> getThemeColor() const;
    void setThemeColor(primitives::ColorRGB<> themeColor);

protected:
    void removeElements(double timeSeconds,
        float viewportWidth,
        float viewportHeight) override;
    void appendElements(double timeSeconds, std::size_t desiredCount) override;

    Cloud generateElement(double startTimeSeconds) override;

    ThemeColorStorage m_themeColor{};
    GenerationDescriptor m_generationDescriptor;
};

} // namespace monsterCatVisualizer::animation

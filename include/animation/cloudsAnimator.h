// Based on particles.js from https://github.com/kaischallenberg/monstercatVisualizer
// Copyright (c) 2025 Kai Schallenberg
// Copyright (c) 2026 Dzmitry Minko
// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>
#include <memory>
#include <random>

#include "utils/themeColorState.h"
#include "models/cloud.h"
#include "animation/containerAnimator.h"

namespace monsterCatVisualizer::animation
{

class CloudsAnimator : public ContainerAnimator<models::Cloud>
{
public:
    static constexpr std::size_t appendLimit = 5;

    using ThemeColorStatePtr = std::shared_ptr<utils::ThemeColorState>;

    struct GenerationDescriptor
    {
        using DistF = std::uniform_real_distribution<float>;

        explicit GenerationDescriptor(const SourceGenerationDescriptor& source);

        DistF radius{};
        DistF themeColor{0.0f, 1.0f};
        float themeColorProbability{};
    };

    CloudsAnimator(ThemeColorStatePtr themeColorState,
        const SourceGenerationDescriptor& source,
        StoragePtr storagePtr);

    ThemeColorStatePtr getThemeColorState() const;

    /**
     * An update is not atomic across the patch check, removal, and append.
     * Intended for one producer and one consumer.
     * No other thread may mutate this storage while the producer is updating it.
     */
    void update(double timeSeconds,
        std::int32_t viewportWidth,
        std::int32_t viewportHeight) override;

protected:
    DataModel generateElement(double timeSeconds,
        std::int32_t viewportWidth,
        std::int32_t viewportHeight) override;

    ThemeColorStatePtr m_themeColorState;
    GenerationDescriptor m_generationDescriptor;
};

} // namespace monsterCatVisualizer::animation

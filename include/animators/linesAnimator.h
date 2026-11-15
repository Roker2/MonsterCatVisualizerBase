// Based on particles.js from https://github.com/kaischallenberg/monstercatVisualizer
// Copyright (c) 2025 Kai Schallenberg
// Copyright (c) 2026 Dzmitry Minko
// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>
#include <memory>
#include <random>

#include "utils/defines.h"
#include "models/line.h"
#include "models/container.h"
#include "animators/baseAnimator.h"

namespace monsterCatVisualizer
{

class LinesAnimator : public BaseAnimator
{
public:
    static constexpr std::size_t appendLimit = 25;

    struct GenerationDescriptor
    {
        using DistF = std::uniform_real_distribution<float>;
        using DistU16 = std::uniform_int_distribution<std::uint16_t>;

        GenerationDescriptor(const LineGenerationDescriptor& lineGenerationDescriptor);

        float elementsDensity{};

        DistF position{0.0f, 1.0f};
        DistF angle{0.0f, utils::twoPi};
        DistF speed{};

        DistF length{};
        float lineWidth{};

        DistU16 colorR{};
        DistU16 colorG{};
        DistU16 colorB{};
        DistF colorA{};
    };

    using GeometryDescriptor = Line::GeometryDescriptor;
    using VisualDescriptor = Line::VisualDescriptor;
    using Storage = Container<Line>;
    using StoragePtr = std::shared_ptr<Storage>;

    LinesAnimator(StoragePtr storagePtr,
        const LineGenerationDescriptor& lineGenerationDescriptor);

    StoragePtr getStorage() const;

    using BaseAnimator::initialize;
    void initialize(std::size_t desiredCount) override;
    void clear() override;
    void update(double timeSeconds,
        float viewportWidth,
        float viewportHeight) override;
    std::size_t calculateDesiredCount(float viewportWidth,
        float viewportHeight) const override;

protected:
    void removeElements(double timeSeconds,
        float viewportWidth,
        float viewportHeight) override;
    void appendElements(double timeSeconds, std::size_t desiredCount) override;

    virtual Line generateLine(double startTimeSeconds);

    StoragePtr m_storagePtr;
    GenerationDescriptor m_generationDescriptor;
};

} // namespace monsterCatVisualizer
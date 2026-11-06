// Based on particles.js from https://github.com/kaischallenberg/monstercatVisualizer
// Copyright (c) 2025 Kai Schallenberg
// Copyright (c) 2026 Dzmitry Minko
// SPDX-License-Identifier: MIT

#pragma once

#include "cloud.h"
#include <vector>
#include <mutex>

namespace monsterCatVisualizer
{

class CloudsContainer
{
public:
    CloudsContainer(uint8_t r = 255, uint8_t g = 0, uint8_t b = 0);
    virtual ~CloudsContainer() = default;

    void generateCloud(float canvasHeight);
    void cleanUp(float canvasHeight, float canvasWidth);
    void update(uint32_t deltaTime, float animationSpeed);
    void setColor(uint8_t r, uint8_t g, uint8_t b);

    virtual void aboutToRemove(const std::vector<size_t>& indices) {};
    virtual void removed() {};

    virtual void updated() {};

    virtual void cloudIsGenerated() {};

protected:
    std::vector<Cloud> m_clouds;
    std::mutex m_mutex;
    uint8_t m_r;
    uint8_t m_g;
    uint8_t m_b;
};

} // namespace monsterCatVisualizer
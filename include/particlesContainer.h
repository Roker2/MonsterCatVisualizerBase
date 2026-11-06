// Based on particles.js from https://github.com/kaischallenberg/monstercatVisualizer
// Copyright (c) 2025 Kai Schallenberg
// Copyright (c) 2026 Dzmitry Minko
// SPDX-License-Identifier: MIT

#pragma once
 
#include "particle.h"
#include <vector>
#include <mutex>

namespace monsterCatVisualizer
{

class ParticlesContainer
{
public:
    ParticlesContainer() = default;
    virtual ~ParticlesContainer() = default;

    void generateParticle(float canvasHeight);
    void cleanUp(float canvasHeight, float canvasWidth);
    void update(uint32_t deltaTime, float animationSpeed);
    std::vector<Particle> getParticles() const;

    virtual void aboutToRemove(const std::vector<size_t>& indices) {};
    virtual void removed() {};

    virtual void updated() {};

    virtual void particleIsGenerated() {};

protected:
    std::vector<Particle> m_particles;
    mutable std::mutex m_mutex;
};

} // namespace monsterCatVisualizer
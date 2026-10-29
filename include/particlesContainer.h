#pragma once
 
#include "particle.h"
#include <vector>

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

    virtual void aboutToRemove(const std::vector<size_t>& indices) {};
    virtual void removed() {};

protected:
    std::vector<Particle> m_particles;
};

} // namespace monsterCatVisualizer
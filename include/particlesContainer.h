#pragma once
 
#include "particle.h"
#include <vector>

namespace monsterCatVisualizer
{

class ParticlesContainer
{
public:
    void generateParticle(float canvasHeight);
    void cleanUp(float canvasHeight, float canvasWidth);
    void update(uint32_t deltaTime, float animationSpeed);

protected:
    std::vector<Particle> m_particles;
};

} // namespace monsterCatVisualizer
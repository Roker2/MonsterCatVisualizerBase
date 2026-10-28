#pragma once

#include "cloudsContainer.h"
#include "linesContainer.h"
#include "particlesContainer.h"

namespace monsterCatVisualizer
{

class Animator
{
public:
    Animator(CloudsContainer cloudsContainer, LinesContainer linesContainer, ParticlesContainer particlesContainer,
             float height, float width);
    void animate(uint32_t deltaTime, float animationSpeed);
private:
    CloudsContainer m_cloudsContainer;
    LinesContainer m_linesContainer;
    ParticlesContainer m_particlesContainer;

    int m_cloudSpawnTimeAccumulator;
    int m_particleSpawnTimeAccumulator;
    int m_lineSpawnTimeAccumulator;

    float m_height;
    float m_width;
};

} // namespace monsterCatVisualizer
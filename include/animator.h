#pragma once

#include "cloudsContainer.h"
#include "linesContainer.h"
#include "particlesContainer.h"

#include <memory>

namespace monsterCatVisualizer
{

class Animator
{
    using CloudsContainerPtr = std::shared_ptr<CloudsContainer>;
    using LinesContainerPtr = std::shared_ptr<LinesContainer>;
    using ParticlesContainerPtr = std::shared_ptr<ParticlesContainer>;

public:
    Animator(CloudsContainerPtr cloudsContainer, LinesContainerPtr linesContainer, ParticlesContainerPtr particlesContainer,
             float height, float width);
    void animate(uint32_t deltaTime, float animationSpeed);
private:
    CloudsContainerPtr m_cloudsContainer;
    LinesContainerPtr m_linesContainer;
    ParticlesContainerPtr m_particlesContainer;

    int m_cloudSpawnTimeAccumulator;
    int m_particleSpawnTimeAccumulator;
    int m_lineSpawnTimeAccumulator;

    float m_height;
    float m_width;
};

} // namespace monsterCatVisualizer
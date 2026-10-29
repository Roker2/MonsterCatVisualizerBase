#include "animator.h"

using namespace monsterCatVisualizer;

static constexpr int baseParticleSpawnInterval = 100;
static constexpr int baseLineSpawnInterval = 1500;
static constexpr int baseCloudSpawnInterval = 400;

Animator::Animator(CloudsContainerPtr cloudsContainer, LinesContainerPtr linesContainer, ParticlesContainerPtr particlesContainer,
                   float height, float width)
    : m_cloudsContainer(cloudsContainer)
    , m_linesContainer(linesContainer)
    , m_particlesContainer(particlesContainer)
    , m_cloudSpawnTimeAccumulator(0)
    , m_particleSpawnTimeAccumulator(0)
    , m_lineSpawnTimeAccumulator(0)
    , m_height(height)
    , m_width(width)
{
}

void Animator::animate(uint32_t deltaTime, float animationSpeed)
{
    int desiredCloudSpawnInterval = static_cast<int>(baseCloudSpawnInterval / animationSpeed);
    int desiredParticleSpawnInterval = static_cast<int>(baseParticleSpawnInterval / animationSpeed);
    int desiredLineSpawnInterval = static_cast<int>(baseLineSpawnInterval / animationSpeed);

    m_cloudSpawnTimeAccumulator += deltaTime;
    m_particleSpawnTimeAccumulator += deltaTime;
    m_lineSpawnTimeAccumulator += deltaTime;

    while (m_cloudSpawnTimeAccumulator >= desiredCloudSpawnInterval)
    {
        m_cloudsContainer->generateCloud(m_height);
        m_cloudSpawnTimeAccumulator -= desiredCloudSpawnInterval;
    }

    while (m_particleSpawnTimeAccumulator >= desiredParticleSpawnInterval)
    {
        m_particlesContainer->generateParticle(m_height);
        m_particleSpawnTimeAccumulator -= desiredParticleSpawnInterval;
    }

    while (m_lineSpawnTimeAccumulator >= desiredLineSpawnInterval)
    {
        m_linesContainer->generateLine(m_height);
        m_lineSpawnTimeAccumulator -= desiredLineSpawnInterval;
    }

    m_cloudsContainer->update(deltaTime, animationSpeed);
    m_particlesContainer->update(deltaTime, animationSpeed);
    m_linesContainer->update(deltaTime, animationSpeed);

    m_cloudsContainer->cleanUp(m_height, m_width);
    m_linesContainer->cleanUp(m_height, m_width);
    m_particlesContainer->cleanUp(m_height, m_width);
}
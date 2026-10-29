#include "particlesContainer.h"
#include "helpers.h"

#include <algorithm>

using namespace monsterCatVisualizer;

void ParticlesContainer::generateParticle(float canvasHeight)
{
    float radius = randomFloat() + 0.5f;
    float x = 0 - radius * (randomFloat() * 2 + 1);
    float yMin = canvasHeight * 0.05f;
    float yMax = canvasHeight * 0.95f;
    float y = yMin + (yMax - yMin) * randomFloat() * 0.5f + (canvasHeight * 0.25f);
    uint8_t r = 255;
    uint8_t g = 255;
    uint8_t b = 255;
    float a = randomFloat() * 0.5f + 0.2f;
    float speedX = randomFloat() * 0.5f + 0.2f;
    float speedY = (randomFloat() - 0.5f) * 0.5f;
    m_particles.emplace_back(x, y, radius, r, g, b, a, speedX, speedY);
    particleIsGenerated();
}

void ParticlesContainer::cleanUp(float canvasHeight, float canvasWidth)
{
    std::vector<size_t> indicesToRemove;
    for (size_t i = 0; i < m_particles.size(); i++)
    {
        const Particle& particle = m_particles[i];
        float maxX = particle.getX() + particle.getRadius();
        float minX = particle.getX() - particle.getRadius();
        float maxY = particle.getY() + particle.getRadius();
        float minY = particle.getY() - particle.getRadius();
        if (minX > canvasWidth ||
            maxY < 0 ||
            minY > canvasHeight)
        indicesToRemove.push_back(i);
    }
    aboutToRemove(indicesToRemove);
    removeIndices(m_particles, indicesToRemove);
    removed();
}

void ParticlesContainer::update(uint32_t deltaTime, float animationSpeed)
{
    for (auto& particle : m_particles)
    {
        particle.update(deltaTime, animationSpeed);
    }
    updated();
}
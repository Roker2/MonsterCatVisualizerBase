#include "cloudsContainer.h"
#include "helpers.h"

#include <algorithm>

using namespace monsterCatVisualizer;

void CloudsContainer::generateCloud(float canvasHeight)
{
    float radius = randomFloat() * 150 + 50;
    float x = 0 - radius * (randomFloat() * 2 + 1);
    float yMin = canvasHeight * 0.4f;
    float yMax = canvasHeight * 0.6f;
    float y = yMin + (yMax - yMin) * randomFloat();
    uint8_t r = 176;
    uint8_t g = 176;
    uint8_t b = 176;
    if (randomFloat() >= 0.6f)
    {
        // red
        r = 255;
        g = 0;
        b = 0;
    }
    float a = 0.01f;
    float speedX = randomFloat() * 0.5f + 0.2f;
    float speedY = (randomFloat() - 0.5f) * 0.5f;
    m_clouds.emplace_back(x, y, radius, r, g, b, a, speedX, speedY);
}

void CloudsContainer::cleanUp(float canvasHeight, float canvasWidth)
{
    std::vector<size_t> indicesToRemove;
    for (size_t i = 0; i < m_clouds.size(); i++)
    {
        const Cloud& cloud = m_clouds[i];
        float maxX = cloud.getX() + cloud.getRadius();
        float minX = cloud.getX() - cloud.getRadius();
        float maxY = cloud.getY() + cloud.getRadius();
        float minY = cloud.getY() - cloud.getRadius();
        if (maxX > canvasWidth ||
            minY < 0 ||
            maxY > canvasHeight)
        indicesToRemove.push_back(i);
    }
    aboutToRemove(indicesToRemove);
    removeIndices(m_clouds, indicesToRemove);
    removed();
}

void CloudsContainer::update(uint32_t deltaTime, float animationSpeed)
{
    for (auto& cloud : m_clouds)
    {
        cloud.update(deltaTime, animationSpeed);
    }
}
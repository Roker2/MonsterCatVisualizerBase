#include "linesContainer.h"
#include "helpers.h"

#include <algorithm>

using namespace monsterCatVisualizer;

void LinesContainer::generateLine(float canvasHeight)
{
    float x1 = 0 - (randomFloat() * 100 + 50);
    float minY = canvasHeight * 0.05f;
    float maxY = canvasHeight * 0.95f;
    float y1 = minY + (maxY - minY) * randomFloat();
    float length = randomFloat() * 4 + 3;
    float angle = (randomFloat() - 0.5f) * 3.14159f / 4;
    // red
    uint8_t r = 255;
    uint8_t g = 0;
    uint8_t b = 0;
    float a = 1.f;
    float speed = randomFloat() * 0.5f + 0.2f;
    m_lines.emplace_back(x1, y1, angle, r, g, b, a, speed, length, 2.f);
    lineIsGenerated();
}

void LinesContainer::cleanUp(float canvasHeight, float canvasWidth)
{
    std::vector<size_t> indicesToRemove;
    for (size_t i = 0; i < m_lines.size(); i++)
    {
        float maxX = std::max(m_lines[i].getX1(), m_lines[i].getX2());
        float minX = std::min(m_lines[i].getX1(), m_lines[i].getX2());
        float maxY = std::max(m_lines[i].getY1(), m_lines[i].getY2());
        float minY = std::min(m_lines[i].getY1(), m_lines[i].getY2());
        if (minX > canvasWidth ||
            maxY < 0 ||
            minY > canvasHeight)
        indicesToRemove.push_back(i);
    }
    aboutToRemove(indicesToRemove);
    removeIndices(m_lines, indicesToRemove);
    removed();
}

void LinesContainer::update(uint32_t deltaTime, float animationSpeed)
{
    for (auto& line : m_lines)
    {
        line.update(deltaTime, animationSpeed);
    }
    updated();
}
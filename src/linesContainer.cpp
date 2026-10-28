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
}

void LinesContainer::cleanUp(float canvasHeight, float canvasWidth)
{
    m_lines.erase(
        std::remove_if(m_lines.begin(), m_lines.end(),
                       [canvasHeight, canvasWidth](const Line& line) {
                           float maxX = std::max(line.getX1(), line.getX2());
                           float minX = std::min(line.getX1(), line.getX2());
                           float maxY = std::max(line.getY1(), line.getY2());
                           float minY = std::min(line.getY1(), line.getY2());

                           if (maxX > canvasWidth ||
                               minY < 0 ||
                               maxY > canvasHeight)
                               return true;
                           return false;
                       }),
        m_lines.end());
}

void LinesContainer::update(uint32_t deltaTime, float animationSpeed)
{
    for (auto& line : m_lines)
    {
        line.update(deltaTime, animationSpeed);
    }
}
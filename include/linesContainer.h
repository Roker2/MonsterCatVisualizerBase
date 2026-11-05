#pragma once

#include "line.h"
#include <vector>

namespace monsterCatVisualizer
{

class LinesContainer
{
public:
    LinesContainer(uint8_t r = 255, uint8_t g = 0, uint8_t b = 0);
    virtual ~LinesContainer() = default;

    void generateLine(float canvasHeight);
    void cleanUp(float canvasHeight, float canvasWidth);
    void update(uint32_t deltaTime, float animationSpeed);
    void setColor(uint8_t r, uint8_t g, uint8_t b);

    virtual void aboutToRemove(const std::vector<size_t>& indices) {};
    virtual void removed() {};

    virtual void updated() {};

    virtual void lineIsGenerated() {};

protected:
    std::vector<Line> m_lines;
    uint8_t m_r;
    uint8_t m_g;
    uint8_t m_b;
};

} // namespace monsterCatVisualizer
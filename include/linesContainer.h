#pragma once

#include "line.h"
#include <vector>

namespace monsterCatVisualizer
{

class LinesContainer
{
public:
    void generateLine(float canvasHeight);
    void cleanUp(float canvasHeight, float canvasWidth);
    void update(uint32_t deltaTime, float animationSpeed);

protected:
    std::vector<Line> m_lines;
};

} // namespace monsterCatVisualizer
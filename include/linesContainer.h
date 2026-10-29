#pragma once

#include "line.h"
#include <vector>

namespace monsterCatVisualizer
{

class LinesContainer
{
public:
    LinesContainer() = default;
    virtual ~LinesContainer() = default;

    void generateLine(float canvasHeight);
    void cleanUp(float canvasHeight, float canvasWidth);
    void update(uint32_t deltaTime, float animationSpeed);

    virtual void aboutToRemove(const std::vector<size_t>& indices) {};
    virtual void removed() {};

protected:
    std::vector<Line> m_lines;
};

} // namespace monsterCatVisualizer
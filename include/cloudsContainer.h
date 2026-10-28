#pragma once

#include "cloud.h"
#include <vector>

namespace monsterCatVisualizer
{

class CloudsContainer
{
public:
    void generateCloud(float canvasHeight);
    void cleanUp(float canvasHeight, float canvasWidth);
    void update(uint32_t deltaTime, float animationSpeed);

protected:
    std::vector<Cloud> m_clouds;
};

} // namespace monsterCatVisualizer
#pragma once

#include "cloud.h"
#include <vector>

namespace monsterCatVisualizer
{

class CloudsContainer
{
public:
    CloudsContainer() = default;
    virtual ~CloudsContainer() = default;

    void generateCloud(float canvasHeight);
    void cleanUp(float canvasHeight, float canvasWidth);
    void update(uint32_t deltaTime, float animationSpeed);

    virtual void aboutToRemove(const std::vector<size_t>& indices) {};
    virtual void removed() {};

protected:
    std::vector<Cloud> m_clouds;
};

} // namespace monsterCatVisualizer
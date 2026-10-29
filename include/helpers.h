#pragma once

#include <random>
#include <algorithm>
#include <vector>

namespace monsterCatVisualizer
{

inline float randomFloat()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    return dis(gen);
}

template<typename T>
void removeIndices(std::vector<T>& vector, const std::vector<size_t>& indicesToRemove)
{
    for (auto it = indicesToRemove.rbegin(); it != indicesToRemove.rend(); ++it)
    {
        const auto vecIt = vector.begin() + *it;
        std::rotate(vecIt, vecIt + 1, vector.end());
    }
    vector.erase(vector.end() - indicesToRemove.size(), vector.end());
}

}
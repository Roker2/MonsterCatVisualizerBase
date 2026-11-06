// Based on particles.js from https://github.com/kaischallenberg/monstercatVisualizer
// Copyright (c) 2025 Kai Schallenberg
// Copyright (c) 2026 Dzmitry Minko
// SPDX-License-Identifier: MIT

#include "linesContainer.h"
#include "helpers.h"

#include <algorithm>

using namespace monsterCatVisualizer;

LinesContainer::LinesContainer(uint8_t r, uint8_t g, uint8_t b)
    : m_r(r), m_g(g), m_b(b)
{
}

void LinesContainer::generateLine(float canvasHeight)
{
    float x1 = 0 - (randomFloat() * 100 + 50);
    float minY = canvasHeight * 0.05f;
    float maxY = canvasHeight * 0.95f;
    float y1 = minY + (maxY - minY) * randomFloat();
    float length = randomFloat() * 4 + 3;
    float angle = (randomFloat() - 0.5f) * 3.14159f / 4;
    uint8_t r = m_r;
    uint8_t g = m_g;
    uint8_t b = m_b;
    float a = 1.f;
    float speed = randomFloat() * 0.5f + 0.2f;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_lines.emplace_back(x1, y1, angle, r, g, b, a, speed, length, 2.f);
    }
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
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        removeIndices(m_lines, indicesToRemove);
    }
    removed();
}

void LinesContainer::update(uint32_t deltaTime, float animationSpeed)
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        for (auto& line : m_lines)
        {
            line.update(deltaTime, animationSpeed);
        }
    }
    updated();
}

void LinesContainer::setColor(uint8_t r, uint8_t g, uint8_t b)
{
    m_r = r;
    m_g = g;
    m_b = b;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        for (auto& line : m_lines)
        {
            line.setColor(r, g, b);
        }
    }
}
#include "line.h"

#include <cmath>

using namespace monsterCatVisualizer;

Line::Line(float x1, float y1, float angle, uint8_t r, uint8_t g, uint8_t b, float a, float speed, float length, float lineWidth)
    : m_x1(x1), m_y1(y1), m_angle(angle), m_r(r), m_g(g), m_b(b), m_a(a), m_speed(speed), m_length(length), m_lineWidth(lineWidth)
{
}

void Line::update(uint32_t deltaTime, float animationSpeed)
{
    const float moveDelta = m_speed * animationSpeed * (deltaTime / 16.67f);
    m_x1 += moveDelta * std::cos(m_angle);
    m_y1 += moveDelta * std::sin(m_angle);
    m_x2 = m_x1 + m_length * std::cos(m_angle);
    m_y2 = m_y1 + m_length * std::sin(m_angle);
}

float Line::getX1() const noexcept
{
    return m_x1;
}

float Line::getY1() const noexcept
{
    return m_y1;
}

float Line::getX2() const noexcept
{
    return m_x2;
}

float Line::getY2() const noexcept
{
    return m_y2;
}

float Line::getAngle() const noexcept
{
    return m_angle;
}

uint8_t Line::getR() const noexcept
{
    return m_r;
}

uint8_t Line::getG() const noexcept
{
    return m_g;
}

uint8_t Line::getB() const noexcept
{
    return m_b;
}

float Line::getA() const noexcept
{
    return m_a;
}

float Line::getSpeed() const noexcept
{
    return m_speed;
}

float Line::getLength() const noexcept
{
    return m_length;
}

float Line::getLineWidth() const noexcept
{
    return m_lineWidth;
}
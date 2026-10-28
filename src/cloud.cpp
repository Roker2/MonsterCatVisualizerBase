#include "cloud.h"

using namespace monsterCatVisualizer;

Cloud::Cloud(float x, float y, float radius, uint8_t r, uint8_t g, uint8_t b, float a, float speedX, float speedY)
    : m_x(x), m_y(y), m_radius(radius), m_r(r), m_g(g), m_b(b), m_a(a), m_speedX(speedX), m_speedY(speedY)
{
}

void Cloud::update(uint32_t deltaTime, float animationSpeed)
{
    m_x += m_speedX * animationSpeed * (deltaTime / 16.67f);
    m_y += m_speedY * animationSpeed * (deltaTime / 16.67f);
}

float Cloud::getX() const noexcept
{
    return m_x;
}

float Cloud::getY() const noexcept
{
    return m_y;
}

float Cloud::getRadius() const noexcept
{
    return m_radius;
}

uint8_t Cloud::getR() const noexcept
{
    return m_r;
}

uint8_t Cloud::getG() const noexcept
{
    return m_g;
}

uint8_t Cloud::getB() const noexcept
{
    return m_b;
}

float Cloud::getA() const noexcept
{
    return m_a;
}

float Cloud::getSpeedX() const noexcept
{
    return m_speedX;
}

float Cloud::getSpeedY() const noexcept
{
    return m_speedY;
}
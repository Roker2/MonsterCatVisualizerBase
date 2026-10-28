#include "particle.h"

using namespace monsterCatVisualizer;

Particle::Particle(float x, float y, float radius, uint8_t r, uint8_t g, uint8_t b, float a, float speedX, float speedY)
    : m_x(x), m_y(y), m_radius(radius), m_r(r), m_g(g), m_b(b), m_a(a), m_speedX(speedX), m_speedY(speedY)
{
}

void Particle::update(uint32_t deltaTime, float animationSpeed)
{
    m_x += m_speedX * animationSpeed * (deltaTime / 16.67f);
    m_y += m_speedY * animationSpeed * (deltaTime / 16.67f);
}

float Particle::getX() const noexcept
{
    return m_x;
}

float Particle::getY() const noexcept
{
    return m_y;
}

float Particle::getRadius() const noexcept
{
    return m_radius;
}

uint8_t Particle::getR() const noexcept
{
    return m_r;
}

uint8_t Particle::getG() const noexcept
{
    return m_g;
}

uint8_t Particle::getB() const noexcept
{
    return m_b;
}

float Particle::getA() const noexcept
{
    return m_a;
}

float Particle::getSpeedX() const noexcept
{
    return m_speedX;
}

float Particle::getSpeedY() const noexcept
{
    return m_speedY;
}
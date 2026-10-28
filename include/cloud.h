#pragma once

#include <stdint.h>

namespace monsterCatVisualizer
{

class Cloud
{
public:
    Cloud(float x, float y, float radius, uint8_t r, uint8_t g, uint8_t b, float a, float speedX, float speedY);
    ~Cloud() = default;

    void update(uint32_t deltaTime, float animationSpeed);

    float getX() const noexcept;
    float getY() const noexcept;
    float getRadius() const noexcept;
    uint8_t getR() const noexcept;
    uint8_t getG() const noexcept;
    uint8_t getB() const noexcept;
    float getA() const noexcept;
    float getSpeedX() const noexcept;
    float getSpeedY() const noexcept;

private:
    float m_x;
    float m_y;
    float m_radius;
    uint8_t m_r;
    uint8_t m_g;
    uint8_t m_b;
    float m_a;
    float m_speedX;
    float m_speedY;
};

} // namespace monsterCatVisualizer
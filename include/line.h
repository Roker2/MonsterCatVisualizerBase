#pragma once

#include <stdint.h>

namespace monsterCatVisualizer
{

class Line
{
public:
    Line(float x1, float y1, float angle, uint8_t r, uint8_t g, uint8_t b, float a, float speed, float length, float lineWidth);
    ~Line() = default;

    void update(uint32_t deltaTime, float animationSpeed);

    float getX1() const noexcept;
    float getY1() const noexcept;
    float getX2() const noexcept;
    float getY2() const noexcept;
    float getAngle() const noexcept;
    uint8_t getR() const noexcept;
    uint8_t getG() const noexcept;
    uint8_t getB() const noexcept;
    float getA() const noexcept;
    float getSpeed() const noexcept;
    float getLength() const noexcept;
    float getLineWidth() const noexcept;

private:
    float m_x1;
    float m_y1;
    float m_x2;
    float m_y2;
    float m_angle;
    uint8_t m_r;
    uint8_t m_g;
    uint8_t m_b;
    float m_a;
    float m_speed;
    float m_length;
    float m_lineWidth;
};

} // namespace monsterCatVisualizer
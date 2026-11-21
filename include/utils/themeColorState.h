// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>
#include <atomic>

#include "primitives/color.h"

namespace monsterCatVisualizer::utils
{

class ThemeColorState
{
public:
    using Color = primitives::ColorRGB<>;

    Color getColor() const noexcept
    {
        return primitives::unpackColor(
            m_color.load(std::memory_order_relaxed));
    }

    void setColor(Color color) noexcept
    {
        m_color.store(
            primitives::packColor(color),
            std::memory_order_relaxed);
    }

private:
    std::atomic<std::uint32_t> m_color{};
};

} // namespace monsterCatVisualizer::utils

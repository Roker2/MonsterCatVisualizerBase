// SPDX-License-Identifier: MIT

#pragma once

#include <atomic>

namespace monsterCatVisualizer::utils
{

class ViewportState
{
public:
    struct ViewportSize
    {
        float width{};
        float height{};
    };

    inline ViewportSize getSize() const noexcept
    {
        return m_size.load(std::memory_order_relaxed);
    }

    inline void setSize(ViewportSize size) noexcept
    {
        m_size.store(size, std::memory_order_relaxed);
    }

private:
    std::atomic<ViewportSize> m_size{};
};

} // namespace monsterCatVisualizer::utils

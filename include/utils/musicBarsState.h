// SPDX-License-Identifier: MIT

#pragma once

#include <mutex>

#include "primitives/color.h"
#include "primitives/vector2.h"

namespace monsterCatVisualizer::utils
{

class MusicBarsState
{
public:
    struct StateDescriptor
    {
        primitives::ColorRGBA<> color{0, 0, 0, 0};
        /** Offset of music bars panel's center in NVS */
        primitives::Vector2<> panelCenterOffset{};
        /** Size of music bars panel in NVS */
        primitives::Vector2<> panelSize{};
        /** Multiplier of each music bar width in range [0, 1] */
        float elementWidthScale{};
    };

    StateDescriptor get() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_state;
    }

    void set(const StateDescriptor& state)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_state = state;
    }

private:
    mutable std::mutex m_mutex;
    StateDescriptor m_state{};
};

} // namespace monsterCatVisualizer::utils

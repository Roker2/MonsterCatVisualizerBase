// SPDX-License-Identifier: MIT

#pragma once

#include <chrono>
#include <mutex>

#include "models/minMax.h"

namespace monsterCatVisualizer::utils
{

class AnimationClock
{
public:
    using Clock = std::chrono::steady_clock;

    struct TimeSample
    {
        double timeSeconds{};
        bool paused{};
    };

    static constexpr MinMax<double> multiplierMinMax{0.01, 100.0};
    static constexpr double defaultSpeedMultiplier{1.0};

    TimeSample getTimeSample() const;
    double getTimeSeconds() const;

    double getSpeedMultiplier() const;
    void setSpeedMultiplier(double multiplier);

    bool isPaused() const;
    void setPaused(bool paused);

private:
    double calculateAnimationTime(Clock::time_point now) const;
    static double validateMultiplier(double multiplier);

    mutable std::mutex m_mutex;
    Clock::time_point m_syncTime{Clock::now()};
    double m_syncAnimationTimeSeconds{};
    double m_speedMultiplier{defaultSpeedMultiplier};
    bool m_pause{};
    bool m_autoPause{};
};

} // namespace monsterCatVisualizer::utils

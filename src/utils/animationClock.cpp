#include "utils/animationClock.h"

#include <algorithm>
#include <cmath>

#include "utils/approximate.h"

namespace monsterCatVisualizer::utils
{

AnimationClock::TimeSample AnimationClock::getTimeSample() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return TimeSample
    {
        calculateAnimationTime(Clock::now()),
        m_pause || m_autoPause
    };
}

double AnimationClock::getTimeSeconds() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return calculateAnimationTime(Clock::now());
}

double AnimationClock::getSpeedMultiplier() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_speedMultiplier;
}

void AnimationClock::setSpeedMultiplier(double multiplier)
{
    multiplier = validateMultiplier(multiplier);
    const bool shouldAutoPause = multiplier == 0.0;

    std::lock_guard<std::mutex> lock(m_mutex);

    const auto now = Clock::now();
    m_syncAnimationTimeSeconds = calculateAnimationTime(now);
    m_syncTime = now;
    m_speedMultiplier = multiplier;
    m_autoPause = shouldAutoPause;
}

bool AnimationClock::isPaused() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_pause || m_autoPause;
}

void AnimationClock::setPaused(bool paused)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_pause == paused)
        return;

    const auto now = Clock::now();
    m_syncAnimationTimeSeconds = calculateAnimationTime(now);
    m_syncTime = now;
    m_pause = paused;
}

double AnimationClock::calculateAnimationTime(Clock::time_point now) const
{
    if (m_pause || m_autoPause)
        return m_syncAnimationTimeSeconds;

    const auto deltaTime =
        std::chrono::duration<double>(now - m_syncTime).count();
    return m_syncAnimationTimeSeconds + deltaTime * m_speedMultiplier;
}

double AnimationClock::validateMultiplier(double multiplier)
{
    if (!std::isfinite(multiplier))
        return defaultSpeedMultiplier;

    const double absValue = std::abs(multiplier);
    if (Approximate::less(absValue, multiplierMinMax.min))
        return 0.0;

    const double sign = multiplier > 0 ? 1.0 : -1.0;
    const double clampedValue = std::clamp(
        absValue,
        multiplierMinMax.min,
        multiplierMinMax.max);
    
    return sign * clampedValue;
}

} // namespace monsterCatVisualizer::utils

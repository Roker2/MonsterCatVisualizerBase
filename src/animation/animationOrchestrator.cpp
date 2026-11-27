// Based on particles.js from https://github.com/kaischallenberg/monstercatVisualizer
// Copyright (c) 2025 Kai Schallenberg
// Copyright (c) 2026 Dzmitry Minko
// SPDX-License-Identifier: MIT

#include "animation/animationOrchestrator.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

#include "utils/mathUtils.h"

namespace monsterCatVisualizer::animation
{

AnimationOrchestrator::AnimationOrchestrator(TaskSchedulerPtr taskScheduler,
    AnimationClockPtr animationClock,
    ViewportStatePtr viewportState)
    : m_taskScheduler(std::move(taskScheduler)),
    m_animationClock(std::move(animationClock)),
    m_viewportState(std::move(viewportState))
{
    if (!m_taskScheduler)
        throw std::runtime_error("AnimationOrchestrator: empty task scheduler");
    if (!m_animationClock)
        throw std::runtime_error("AnimationOrchestrator: empty animation clock");
    if (!m_viewportState)
        throw std::runtime_error("AnimationOrchestrator: empty viewport state");
}

AnimationOrchestrator::~AnimationOrchestrator()
{
    removeAnimators();
}

const AnimationOrchestrator::Storage& AnimationOrchestrator::getStorage() const
{
    return m_animators;
}

void AnimationOrchestrator::addAnimator(AnimatorPtr animator,
    double updateIntervalSeconds)
{
    if (!animator)
        return;

    const auto it = std::find_if(m_animators.begin(), m_animators.end(),
        [animator](const auto& descriptor) { return descriptor.animator == animator; });

    if (it != m_animators.end())
        throw std::runtime_error(
            "AnimationOrchestrator: adding the same animator twice is not allowed");

    const auto viewportSize = m_viewportState->getSize();
    animator->initialize(viewportSize.width, viewportSize.height);

    const auto taskId = m_taskScheduler->addTask(
        [animator, animationClock = m_animationClock, viewportState = m_viewportState]()
        {
            const auto viewportSize =
                viewportState->getSize();
            const auto timeSample =
                animationClock->getTimeSample();

            if (!utils::SpatialMathUtils::viewportSizeValid(
                viewportSize.width, viewportSize.height))
                return;
            if (timeSample.paused)
                return;

            animator->update(timeSample.timeSeconds,
                viewportSize.width, viewportSize.height);
        }, updateIntervalSeconds);

    m_animators.emplace_back(AnimatorDescriptor{std::move(animator), taskId});
}

void AnimationOrchestrator::removeAnimator(AnimatorPtr animator)
{
    const auto it = std::find_if(m_animators.begin(), m_animators.end(),
        [animator](const auto& descriptor) { return descriptor.animator == animator; });

    if (it != m_animators.end())
    {
        m_taskScheduler->removeTask(it->taskId);
        m_animators.erase(it);
    }
}

void AnimationOrchestrator::removeAnimator(utils::TaskScheduler::TaskId taskId)
{
    const auto it = std::find_if(m_animators.begin(), m_animators.end(),
        [taskId](const auto& descriptor) { return descriptor.taskId == taskId; });

    if (it != m_animators.end())
    {
        m_taskScheduler->removeTask(it->taskId);
        m_animators.erase(it);
    }
}

void AnimationOrchestrator::removeAnimators()
{
    for (const auto& [_, taskId] : m_animators)
    {
        m_taskScheduler->removeTask(taskId);
    }
    m_animators.clear();
}

} // namespace monsterCatVisualizer::animation

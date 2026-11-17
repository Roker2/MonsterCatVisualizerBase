// Based on particles.js from https://github.com/kaischallenberg/monstercatVisualizer
// Copyright (c) 2025 Kai Schallenberg
// Copyright (c) 2026 Dzmitry Minko
// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <vector>

#include "utils/animationClock.h"
#include "utils/viewportState.h"
#include "utils/taskScheduler.h"
#include "animation/baseAnimator.h"

namespace monsterCatVisualizer::animation
{

class AnimationOrchestrator
{
public:
    struct AnimatorDescriptor
    {
        using AnimatorPtr = std::shared_ptr<BaseAnimator>;

        AnimatorPtr animator{};
        utils::TaskScheduler::TaskId taskId{};
    };

    using AnimatorPtr = AnimatorDescriptor::AnimatorPtr;
    using Storage = std::vector<AnimatorDescriptor>;
    using AnimationClockPtr = std::shared_ptr<utils::AnimationClock>;
    using ViewportStatePtr = std::shared_ptr<utils::ViewportState>;

    AnimationOrchestrator(AnimationClockPtr animationClock,
        ViewportStatePtr viewportState);

    const Storage& getStorage() const;

    /**
     * Add, initialize and schedule an animator.
     * ViewportState must contain a valid size before initialization.
     */
    void addAnimator(AnimatorPtr animator, double updateIntervalSeconds);
    void removeAnimator(AnimatorPtr animator);
    void removeAnimator(utils::TaskScheduler::TaskId taskId);
    void removeAllAnimators();

private:
    Storage m_animators;
    utils::TaskScheduler m_taskScheduler;
    AnimationClockPtr m_animationClock;
    ViewportStatePtr m_viewportState;
};

} // namespace monsterCatVisualizer::animators
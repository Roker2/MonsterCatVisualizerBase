// SPDX-License-Identifier: MIT

#pragma once

#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

namespace monsterCatVisualizer::utils
{

class TaskScheduler
{
public:
    using TaskId = std::uint32_t;

    explicit TaskScheduler(std::size_t threadsCount = std::thread::hardware_concurrency());
    ~TaskScheduler();

    TaskScheduler(const TaskScheduler&) = delete;
    TaskScheduler& operator=(const TaskScheduler&) = delete;

    /**
     * Runs the callback periodically. The interval is measured from the start
     * of one invocation to the start of the next; overlapping runs are forbidden.
     * Callbacks must not throw exceptions.
     */
    TaskId addTask(std::function<void()> callback, double updateIntervalSeconds);

    /**
     * Prevents further runs. A currently running callback finishes normally.
     * The task's captured objects remain alive until that callback completes.
     */
    bool removeTask(TaskId id);

    /**
     * Stops scheduling new callbacks and joins all workers.
     * Must be called from outside worker threads and not concurrently.
     * Callbacks must terminate.
     */
    void stop();

private:
    using Clock = std::chrono::steady_clock;

    enum class TaskState
    {
        Waiting,
        Running,
        Removed
    };

    struct Task
    {
        TaskId id;
        std::function<void()> callback;
        Clock::duration interval;
        Clock::time_point nextRunTime;
        TaskState state{TaskState::Waiting};
    };

    static constexpr std::size_t maxTaskSearchCount = 16;
    static constexpr auto searchRetryInterval = std::chrono::milliseconds{100};

    void workerLoop();
    void eraseTaskAt(std::size_t index);

    std::vector<std::thread> m_threads;
    std::vector<std::shared_ptr<Task>> m_tasks;

    mutable std::mutex m_mutex;
    std::condition_variable m_cv;

    std::size_t m_nextTaskIndex{};
    std::uint64_t m_nextTaskId{1};
    bool m_stopRequested{};
};

} // namespace monsterCatVisualizer::utils

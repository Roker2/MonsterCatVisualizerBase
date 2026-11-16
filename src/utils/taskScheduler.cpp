// SPDX-License-Identifier: MIT

#include "utils/taskScheduler.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <utility>

namespace monsterCatVisualizer::utils
{

TaskScheduler::TaskScheduler(std::size_t threadsCount)
{
    threadsCount = std::max<std::size_t>(threadsCount, 1);
    m_threads.reserve(threadsCount);

    try
    {
        for (std::size_t i = 0; i < threadsCount; ++i)
            m_threads.emplace_back([this] { workerLoop(); });
    }
    catch (...)
    {
        stop();
        throw;
    }
}

TaskScheduler::~TaskScheduler()
{
    stop();
}

TaskScheduler::TaskId TaskScheduler::addTask(
    std::function<void()> callback, double updateIntervalSeconds)
{
    if (!callback || !std::isfinite(updateIntervalSeconds) ||
        updateIntervalSeconds <= 0.0 ||
        updateIntervalSeconds >= std::chrono::duration<double>(Clock::duration::max() / 2).count())
        throw std::invalid_argument("TaskScheduler: invalid task or interval");

    const auto interval = std::max(Clock::duration{1},
        std::chrono::duration_cast<Clock::duration>(
            std::chrono::duration<double>{updateIntervalSeconds}));

    TaskId id;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_stopRequested)
            throw std::logic_error("TaskScheduler: cannot add a task after stop");
        if (m_nextTaskId > std::numeric_limits<TaskId>::max())
            throw std::overflow_error("TaskScheduler: task IDs exhausted");

        id = static_cast<TaskId>(m_nextTaskId);
        auto task = std::make_shared<Task>(Task{id, std::move(callback),
            interval, Clock::now(), TaskState::Waiting});
        m_tasks.push_back(std::move(task));
        ++m_nextTaskId;
    }
    m_cv.notify_all();
    return id;
}

bool TaskScheduler::removeTask(TaskId id)
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        const auto it = std::find_if(m_tasks.begin(), m_tasks.end(),
            [id](const auto& task) { return task->id == id; });
        if (it == m_tasks.end() || (*it)->state == TaskState::Removed)
            return false;

        if ((*it)->state == TaskState::Waiting)
        {
            eraseTaskAt(static_cast<std::size_t>(it - m_tasks.begin()));
        }
        else
        {
            (*it)->state = TaskState::Removed;
        }
    }
    m_cv.notify_all();
    return true;
}

void TaskScheduler::stop()
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_stopRequested = true;
    }
    m_cv.notify_all();

    for (auto& thread : m_threads)
    {
        if (thread.joinable())
            thread.join();
    }

    std::lock_guard<std::mutex> lock(m_mutex);
    m_tasks.clear();
    m_nextTaskIndex = 0;
}

void TaskScheduler::workerLoop()
{
    while (true)
    {
        std::shared_ptr<Task> selected;
        Clock::time_point startTime;

        {
            std::unique_lock<std::mutex> lock(m_mutex);
            while (!m_stopRequested && !selected)
            {
                if (m_tasks.empty())
                {
                    m_cv.wait(lock);
                    continue;
                }

                const std::size_t size = m_tasks.size();
                const std::size_t searchCount = std::min(size, maxTaskSearchCount);
                const auto now = Clock::now();
                auto nextWakeTime = Clock::time_point::max();

                for (std::size_t i = 0; i < searchCount; ++i)
                {
                    const std::size_t index = (m_nextTaskIndex + i) % size;
                    auto& task = m_tasks[index];
                    if (task->state != TaskState::Waiting)
                        continue;

                    if (now >= task->nextRunTime)
                    {
                        task->state = TaskState::Running;
                        m_nextTaskIndex = (index + 1) % size;
                        selected = task;
                        startTime = Clock::now();
                        break;
                    }
                    nextWakeTime = std::min(nextWakeTime, task->nextRunTime);
                }

                if (selected)
                    break;

                if (searchCount < size)
                {
                    // Continue with another slice even without notifications.
                    m_nextTaskIndex = (m_nextTaskIndex + searchCount) % size;
                    const auto retryTime = Clock::now() + searchRetryInterval;
                    m_cv.wait_until(lock, std::min(nextWakeTime, retryTime));
                }
                else if (nextWakeTime != Clock::time_point::max())
                {
                    m_cv.wait_until(lock, nextWakeTime);
                }
                else
                {
                    // All tasks are running or removed. A worker will notify us.
                    m_cv.wait(lock);
                }
            }

            if (m_stopRequested)
                return;
        }

        selected->callback();
        const auto finishTime = Clock::now();

        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (selected->state == TaskState::Removed)
            {
                const auto it = std::find(m_tasks.begin(), m_tasks.end(), selected);
                if (it != m_tasks.end())
                    eraseTaskAt(static_cast<std::size_t>(it - m_tasks.begin()));
            }
            else
            {
                selected->nextRunTime = std::max(startTime + selected->interval, finishTime);
                selected->state = TaskState::Waiting;
            }
        }
        m_cv.notify_all();
    }
}

void TaskScheduler::eraseTaskAt(std::size_t index)
{
    m_tasks.erase(m_tasks.begin() + static_cast<std::ptrdiff_t>(index));
    if (m_tasks.empty())
    {
        m_nextTaskIndex = 0;
    }
    else
    {
        if (index < m_nextTaskIndex)
            --m_nextTaskIndex;
        if (m_nextTaskIndex >= m_tasks.size())
            m_nextTaskIndex = 0;
    }
}

} // namespace monsterCatVisualizer::utils

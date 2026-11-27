// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <mutex>
#include <utility>
#include <vector>

#include "utils/typeTraits.h"

namespace monsterCatVisualizer::models
{

template <typename T>
class SimpleContainer
{
public:
    using DataStorage = std::vector<T>;

    std::size_t getDataSize() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_data.size();
    }

    void getData(DataStorage& result) const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        result = m_data;
    }

    void setData(const DataStorage& data)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_data = data;
    }
    void setData(DataStorage&& data)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_data = std::move(data);
    }

    void reserve(std::size_t capacity)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_data.reserve(capacity);
    }

    void clear()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_data.clear();
    }

    bool removeAt(std::size_t index, std::size_t count = 1)
    {
        if (count == 0)
            return true;

        std::lock_guard<std::mutex> lock(m_mutex);

        if (index > m_data.size() || count > m_data.size() - index)
            return false;

        const auto begin = m_data.begin() + static_cast<std::ptrdiff_t>(index);
        const auto end = begin + static_cast<std::ptrdiff_t>(count);
        m_data.erase(begin, end);
        return true;
    }

    void append(const T& value)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_data.push_back(value);
    }

    template <typename U>
    void append(const U& elements)
    {
        if (elements.empty())
            return;

        std::lock_guard<std::mutex> lock(m_mutex);
        m_data.insert(m_data.end(), elements.begin(), elements.end());
    }

    /**
     * Invokes the callback while holding the container mutex.
     * The callback must not call other methods of this container.
     */
    template <typename F>
    void doForStorage(F&& func)
    {
        if constexpr (utils::is_null_comparable_v<F>)
        {
            if (func == nullptr)
                return;
        }

        std::lock_guard<std::mutex> lock(m_mutex);
        func(m_data);
    }

private:
    mutable std::mutex m_mutex;
    DataStorage m_data{};
};

} // namespace monsterCatVisualizer::models

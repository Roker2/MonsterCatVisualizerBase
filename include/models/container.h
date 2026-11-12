// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <algorithm>
#include <mutex>
#include <utility>
#include <vector>

#include "utils/typeTraits.h"
#include "operation.h"

namespace monsterCatVisualizer
{

template <typename T>
class Container
{
    using DataStorage = std::vector<T>;
    using PatchStorage = Patch<T>;

public:
    static constexpr std::size_t defaultPatchRemoveOperationsCapacity = 512ull;
    static constexpr float defaultPatchAppendedCapacityCoefficient = 0.5f;

    void getData(DataStorage& result) const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        result = m_data;
    }

    /** Get initial data and discard pending changes. */
    void getInitialData(DataStorage& result)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        result = m_data;
        m_hasPatch = false;
        m_patch.reset();
    }

    bool hasPatch() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_hasPatch;
    }

    void getPatch(PatchStorage& result) const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        result = m_patch;
    }

    bool acquirePatch(PatchStorage& result)
    {
        result.clear();

        std::lock_guard<std::mutex> lock(m_mutex);

        if (!m_hasPatch)
            return false;

        m_hasPatch = false;
        std::swap(result, m_patch);

        return true;
    }

    /** 0. Usually called on initialization. */
    void reserve(std::size_t capacity,
        bool reservePatch = true,
        std::size_t patchRemoveOperationsCapacity = defaultPatchRemoveOperationsCapacity,
        float patchAppendedCapacityCoefficient = defaultPatchAppendedCapacityCoefficient)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_data.reserve(capacity);
        if (reservePatch)
        {
            m_patch.removeOperations.reserve(patchRemoveOperationsCapacity);
            m_patch.appended.reserve(static_cast<std::size_t>(
                capacity * std::clamp(patchAppendedCapacityCoefficient, 0.01f, 1.0f)));
        }
    }

    /** 1. Removes all stored data, including earlier changes in the pending patch. */
    void clear()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_hasPatch = true;
        m_patch.reset();
        m_patch.clearFlag = true;
        m_data.clear();
    }

    /** 2. Remove a range; removal after append is not allowed. */
    bool removeAt(std::size_t index, std::size_t count = 1)
    {
        if (count == 0)
            return true;

        std::lock_guard<std::mutex> lock(m_mutex);

        if (m_hasPatch && m_patch.clearFlag)
            return true;
        if (m_hasPatch && !m_patch.appended.empty())
            return false;

        if (index > m_data.size() || count > m_data.size() - index)
            return false;

        m_hasPatch = true;
        const OperationRemove operation{index, count};
        m_patch.removeOperations.push_back(operation);
        return applyRemoveOperation(m_data, operation);
    }

    /** 2. Remove matching elements; removal after append is not allowed. */
    template <typename P>
    bool removeIf(P&& predicate)
    {
        if constexpr (utils::is_null_comparable<P>::value)
        {
            if (predicate == nullptr)
                return false;
        }

        std::lock_guard<std::mutex> lock(m_mutex);
        return removeIfLocked(predicate);
    }

    /** 3. Append a single element. */
    void append(const T& value)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_hasPatch = true;
        m_patch.appended.push_back(value);
        m_data.push_back(value);
    }

    /** 3. Append a range. */
    template <typename U>
    void append(const U& elements)
    {
        if (elements.empty())
            return;

        std::lock_guard<std::mutex> lock(m_mutex);
        m_hasPatch = true;
        m_patch.appended.insert(
            m_patch.appended.end(), elements.begin(), elements.end());
        m_data.insert(m_data.end(), elements.begin(), elements.end());
    }

private:
    template <typename P>
    bool removeIfLocked(P&& predicate)
    {
        if (m_hasPatch && m_patch.clearFlag)
            return true;
        if (m_hasPatch && !m_patch.appended.empty())
            return false;

        // Collect ranges before mutating m_data, so no iterator is invalidated.
        // Scan backwards to ensure tail elements were inspected before removal.
        auto& operations = m_patch.removeOperations;
        const std::size_t firstNewOperation = operations.size();

        OperationRemove current{};

        for (std::size_t i = m_data.size(); i > 0; --i)
        {
            if (predicate(std::as_const(m_data[i - 1])))
            {
                current.index = i - 1;
                ++current.count;
            }
            else if (current.count > 0)
            {
                operations.push_back(current);
                current = {};
            }
        }

        if (current.count > 0)
            operations.push_back(current);

        if (operations.size() == firstNewOperation)
            return true;

        m_hasPatch = true;

        for (std::size_t i = firstNewOperation; i < operations.size(); ++i)
        {
            if (!applyRemoveOperation(m_data, operations[i]))
                return false;
        }

        return true;
    }

    DataStorage m_data{};
    PatchStorage m_patch{};
    bool m_hasPatch{};
    mutable std::mutex m_mutex;
};

} // namespace monsterCatVisualizer

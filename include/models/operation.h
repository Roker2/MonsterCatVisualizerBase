// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <iterator>
#include <utility>
#include <vector>

namespace monsterCatVisualizer
{

struct OperationRemove
{
    std::size_t index{};
    std::size_t count{};
};

template <typename T>
struct Patch
{
    void reset() noexcept
    {
        removeOperations.clear();
        appended.clear();
        clearFlag = false;
    }

    std::vector<OperationRemove> removeOperations;
    std::vector<T> appended;
    bool clearFlag{};
};

template <typename Container>
bool applyRemoveOperation(Container& data, const OperationRemove& operation)
{
    if (operation.index > data.size() ||
        operation.count > data.size() - operation.index)
        return false;

    if (operation.count == 0)
        return true;

    const auto begin = data.begin() + static_cast<std::ptrdiff_t>(operation.index);
    const auto end = begin + static_cast<std::ptrdiff_t>(operation.count);
    const std::size_t tailSize = data.size() - operation.index - operation.count;

    if (operation.count >= tailSize)
    {
        // Erase the range directly when it is at the end or moving the tail is cheaper.
        data.erase(begin, end);
    }
    else
    {
        // Move the last 'count' elements into the removed range in reverse order.
        // The source and destination ranges cannot overlap because count < tailSize.
        auto last = data.rbegin();
        for (std::size_t i = operation.count; i > 0; --i, ++last)
            data[operation.index + i - 1] = std::move(*last);

        const auto tailBegin = data.end() - static_cast<std::ptrdiff_t>(operation.count);
        data.erase(tailBegin, data.end());
    }

    return true;
}

template <typename Container, typename T>
bool applyPatch(Container& data, Patch<T>&& patch)
{
    if (patch.clearFlag)
    {
        data.clear();
    }
    else
    {
        for (const auto& operation : patch.removeOperations)
        {
            if (!applyRemoveOperation(data, operation))
                return false;
        }
    }

    data.insert(
        data.end(),
        std::make_move_iterator(patch.appended.begin()),
        std::make_move_iterator(patch.appended.end()));

    return true;
}

} // namespace monsterCatVisualizer

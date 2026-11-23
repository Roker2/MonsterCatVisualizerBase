// SPDX-License-Identifier: MIT

#pragma once

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <set>
#include <type_traits>
#include <utility>
#include <vector>

namespace monsterCatVisualizer::models
{

struct IndexRange
{
    std::size_t index{};
    std::size_t count{};
};

struct IndexRangeComparator
{
    inline bool operator()(const IndexRange& lhs, const IndexRange& rhs) const
    {
        return lhs.index < rhs.index;
    }
};

template <typename T>
struct Patch
{
    bool empty() const noexcept
    {
        return !clearFlag && removeOperations.empty() && appended.empty();
    }

    void reset() noexcept
    {
        clearFlag = false;
        removeOperations.clear();
        appended.clear();
        modifiedRanges.clear();
    }

    bool clearFlag{};
    std::vector<IndexRange> removeOperations;
    std::vector<T> appended;

    std::set<IndexRange, IndexRangeComparator> modifiedRanges;
};

template <typename ModifiedRanges>
void addToModifiedRanges(const IndexRange& operation,
    ModifiedRanges& modifiedRanges)
{
    if (operation.count == 0)
        return;

    std::size_t begin = operation.index;
    std::size_t end = operation.index + operation.count;

    auto it = modifiedRanges.lower_bound(IndexRange{begin, 0});

    if (it != modifiedRanges.begin())
    {
        const auto previous = std::prev(it);
        if (previous->index + previous->count >= begin)
            it = previous;
    }

    while (it != modifiedRanges.end() && it->index <= end)
    {
        begin = std::min(begin, it->index);
        end = std::max(end, it->index + it->count);
        it = modifiedRanges.erase(it);
    }

    modifiedRanges.insert(it, IndexRange{begin, end - begin});
}

template <typename Container, typename ModifiedRanges = std::nullptr_t>
bool applyRemoveOperation(Container& data,
    const IndexRange& operation,
    ModifiedRanges* modifiedRanges = nullptr)
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

        if constexpr (!std::is_same_v<ModifiedRanges, std::nullptr_t>)
        {
            if (modifiedRanges)
            {
                addToModifiedRanges(IndexRange{operation.index, tailSize}, *modifiedRanges);
            }
        }
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

        if constexpr (!std::is_same_v<ModifiedRanges, std::nullptr_t>)
        {
            if (modifiedRanges)
            {
                addToModifiedRanges(IndexRange{operation.index, operation.count}, *modifiedRanges);
            }
        }
    }

    return true;
}

} // namespace monsterCatVisualizer::models

// SPDX-License-Identifier: MIT

#include "animation/musicBarsAnimator.h"

#include <cmath>
#include <random>
#include <stdexcept>

#include "utils/defines.h"

namespace
{

// TODO: remove temp random generation of music bars
void generateRandomMusicBars(std::vector<float>& storage)
{
    static constexpr float sinePeriods = 3.0f;
    static constexpr float randomRadius = 0.02f;

    thread_local std::mt19937 rng{std::random_device{}()};
    thread_local std::uniform_real_distribution<float> dist{-randomRadius, randomRadius};

    if (storage.empty())
        return;

    const std::size_t count = storage.size();
    const float delta = 1.0f / count;

    for (std::size_t index = 0; index < count; ++index)
    {
        const float relativeIndex = (index + 0.5f) * delta;
        const float sineArg = relativeIndex * monsterCatVisualizer::utils::twoPi * sinePeriods;
        const float sineResult = std::sin(sineArg - monsterCatVisualizer::utils::pi * 0.5f) * 0.4f + 0.5f;

        storage[index] = sineResult + dist(rng);
    }
}

} // namespace

namespace monsterCatVisualizer::animation
{

MusicBarsAnimator::MusicBarsAnimator(MusicBarsStatePtr musicBarsState,
    StoragePtr storagePtr)
    : m_musicBarsState(std::move(musicBarsState)),
    m_storage(std::move(storagePtr))
{
    if (!m_musicBarsState)
        throw std::runtime_error("MusicBarsAnimator: empty musicBarsState");
    if (!m_storage)
        throw std::runtime_error("MusicBarsAnimator: empty storage");
}

MusicBarsAnimator::MusicBarsStatePtr MusicBarsAnimator::getMusicBarsState() const noexcept
{
    return m_musicBarsState;
}

MusicBarsAnimator::StoragePtr MusicBarsAnimator::getStorage() const noexcept
{
    return m_storage;
}

void MusicBarsAnimator::initialize(std::int32_t /* viewportWidth */,
    std::int32_t /* viewportHeight */)
{
    m_storage->doForStorage([](auto& storage)
        {
            storage.assign(defaultDesiredCount, 0.0f);
        });
}

void MusicBarsAnimator::clear()
{
    m_storage->clear();
}

void MusicBarsAnimator::update(double /* timeSeconds */,
    std::int32_t /* viewportWidth */,
    std::int32_t /* viewportHeight */)
{
    m_storage->doForStorage([](auto& storage)
        {
            generateRandomMusicBars(storage);
        });
}

} // namespace monsterCatVisualizer::animation

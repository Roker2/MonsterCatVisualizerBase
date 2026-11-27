// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>

#include "utils/musicBarsState.h"
#include "models/simpleContainer.h"
#include "animation/iAnimator.h"

namespace monsterCatVisualizer::animation
{

class MusicBarsAnimator : public IAnimator
{
public:
    using MusicBarsStatePtr = std::shared_ptr<utils::MusicBarsState>;
    using Storage = models::SimpleContainer<float>;
    using StoragePtr = std::shared_ptr<Storage>;

    static constexpr std::size_t defaultDesiredCount{63};

    MusicBarsAnimator(MusicBarsStatePtr musicBarsState,
        StoragePtr storagePtr);

    MusicBarsStatePtr getMusicBarsState() const noexcept;

    StoragePtr getStorage() const noexcept;

    void initialize(std::int32_t viewportWidth,
        std::int32_t viewportHeight) override;
    void clear() override;

    // TODO: rework method update, that gives real picture of music bars
    void update(double timeSeconds,
        std::int32_t viewportWidth,
        std::int32_t viewportHeight) override;

private:
    MusicBarsStatePtr m_musicBarsState{};
    StoragePtr m_storage;
};

} // namespace monsterCatVisualizer::animation

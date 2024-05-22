#ifndef SYSTEM_MOVEMENT_CPP
#define SYSTEM_MOVEMENT_CPP

#include <iostream>

#include <components.hpp>
#include <systems.hpp>
#include <utilities.hpp>

RenderSystem::RenderSystem(std::shared_ptr<ecs::Coordinator> coordinator, Renderer const& renderer) : ecs::ISystem(coordinator), mRenderer(renderer) {}

void RenderSystem::Initialize(std::array<std::pair<std::string_view, Point>, config::kMaxSpriteSheetID> const& mSpriteSheetData) {
    auto itl = mSpriteSheetData.begin();
    auto itr = mSpriteSheets.begin();

    while (itl != mSpriteSheetData.end() && itr != mSpriteSheets.end()) {
        itr->Initialize(mRenderer, itl->first, itl->second);
        ++itr;
        ++itl;
    }
}

void RenderSystem::Integrate(std::uint32_t dt) {
    (void)dt;   // Suppress unused parameter warning

    for (auto const& entityID : mEntityIDs) {
        auto& transform = mCoordinator->GetComponent<components::Transform>(entityID);
        auto& sprite = mCoordinator->GetComponent<components::Sprite>(entityID);

        auto spriteSheet = mSpriteSheets[sprite.spriteSheetID];
        auto srcRect = spriteSheet.GetSrcRect(sprite.spriteID);
        auto dstRect = GetDstRect(srcRect, transform.position);

        mRenderer.RenderCopy(spriteSheet, srcRect, dstRect);
    }
}

Rect RenderSystem::GetDstRect(Rect const& srcRect, Point const& position) noexcept {
    return Rect({
        position.x - (srcRect.w >> 1),
        position.y - (srcRect.h >> 1),
        srcRect.w,
        srcRect.h,
    });
}


#endif
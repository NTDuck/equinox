#ifndef SYSTEMS_RENDER_CPP
#define SYSTEMS_RENDER_CPP

#include <components.hpp>
#include <systems.hpp>
#include <utilities.hpp>


systems::Render::Render(std::shared_ptr<ecs::Coordinator> coordinator, Renderer const& renderer) : ecs::ISystem(coordinator), mRenderer(renderer) {}

void systems::Render::Initialize(std::array<std::pair<std::string_view, Point>, config::kMaxSpriteSheetID> const& mSpriteSheetData) {
    auto it = mSpriteSheets.begin();
    for (auto const& [filePath, spriteSize] : mSpriteSheetData)
        it++->Initialize(mRenderer, filePath, spriteSize);
}

void systems::Render::Integrate() const {
    for (auto const& entityID : mEntityIDs) {
        auto& position = mCoordinator->GetMember<components::Transform, 0>(entityID);
        auto& spriteSheetID = mCoordinator->GetMember<components::Sprite, 0>(entityID);
        auto& spriteID = mCoordinator->GetMember<components::Sprite, 1>(entityID);

        auto spriteSheet = mSpriteSheets[spriteSheetID];
        auto srcRect = spriteSheet.GetSrcRect(spriteID);
        auto dstRect = GetDstRect(srcRect, position);

        mRenderer.RenderCopy(spriteSheet, srcRect, dstRect);
    }
}

Rect systems::Render::GetDstRect(Rect const& srcRect, Point const& position) noexcept {
    return Rect({
        position.x - (srcRect.w >> 1),
        position.y - (srcRect.h >> 1),
        srcRect.w,
        srcRect.h,
    });
}


#endif
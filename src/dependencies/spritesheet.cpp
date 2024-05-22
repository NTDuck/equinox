#ifndef DEPENDENCIES_SPRITESHEET_CPP
#define DEPENDENCIES_SPRITESHEET_CPP

#include <cassert>
#include <filesystem>

#include <application.hpp>


void SpriteSheet::Initialize(Renderer const& renderer, std::string_view filePath, Point const& spriteSize) {
    assert(std::filesystem::exists(filePath) && "Spritesheet file path does not exist");

    utility::Pointer<SDL_Texture, IMG_LoadTexture, SDL_DestroyTexture>::Emplace(renderer.Get(), filePath.data());

    mSpriteSize = spriteSize;
    SDL_QueryTexture(Get(), nullptr, nullptr, &mSpriteColumnCount, nullptr);
    mSpriteColumnCount /= mSpriteSize.x;

    assert(mSpriteSize > 0 && "Sprite size must be positive integers");
    assert(mSpriteColumnCount > 0 && "Sprite column count must be positive integers");
}

Rect SpriteSheet::GetSrcRect(SpriteID spriteID) const noexcept {
    return Rect{
        spriteID % mSpriteColumnCount * mSpriteSize.x,
        spriteID / mSpriteColumnCount * mSpriteSize.y,
        mSpriteSize.x,
        mSpriteSize.y,
    };
}


#endif
#ifndef SERVICES_SPRITESHEET_CPP
#define SERVICES_SPRITESHEET_CPP

#include <cassert>
#include <filesystem>

#include <services.hpp>


void SpriteSheet::Initialize(Renderer const& renderer, std::string_view filePath, Point const& spriteSize) {
    assert(std::filesystem::exists(filePath) && "Spritesheet file path does not exist");

    utility::SmartPointer<SDL_Texture, IMG_LoadTexture, SDL_DestroyTexture>::Reset(renderer.Get(), filePath.data());

    assert(mSpriteSize > 0 && "Sprite size must be positive integers");
    mSpriteSize = spriteSize;
    
    SDL_QueryTexture(Get(), nullptr, nullptr, &mSpriteColumnCount, nullptr);
    assert(mSpriteColumnCount > 0 && "Sprite column count must be a positive integer");
    mSpriteColumnCount /= mSpriteSize.x;
}

Rect SpriteSheet::GetSrcRect(SpriteID spriteID) const noexcept {
    return {
        spriteID % mSpriteColumnCount * mSpriteSize.x,
        spriteID / mSpriteColumnCount * mSpriteSize.y,
        mSpriteSize.x,
        mSpriteSize.y,
    };
}


#endif
#ifndef DEPENDENCIES_RENDERER_CPP
#define DEPENDENCIES_RENDERER_CPP

#include <dependencies.hpp>


void Renderer::Initialize(Window const& window, std::int32_t index, std::uint32_t flags) {
    utility::Pointer<SDL_Renderer, SDL_CreateRenderer, SDL_DestroyRenderer>::Emplace(window.Get(), index, flags);
}

void Renderer::Clear() const {
    SDL_RenderClear(Get());
}

void Renderer::RenderPresent() const {
    SDL_RenderPresent(Get());
}

void Renderer::SetDrawColor(Color const& color) const {
    SDL_SetRenderDrawColor(Get(), color.r, color.g, color.b, color.a);
}

void Renderer::FillRect(Rect const& rect) const {
    SDL_RenderFillRect(Get(), &rect);
}

void Renderer::FillRect() const {
    SDL_RenderFillRect(Get(), nullptr);
}

void Renderer::RenderCopy(SpriteSheet const& spriteSheet, Rect const& srcRect, Rect const& destRect) const {
    SDL_RenderCopy(Get(), spriteSheet.Get(), &srcRect, &destRect);
}


#endif
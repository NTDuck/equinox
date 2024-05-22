#ifndef DEPENDENCIES_WINDOW_CPP
#define DEPENDENCIES_WINDOW_CPP

#include <dependencies.hpp>


void Window::Initialize(std::string_view const& title, Rect const& size, std::uint32_t flags) {
    utility::Pointer<SDL_Window, SDL_CreateWindow, SDL_DestroyWindow>::Emplace(title.data(), size.x, size.y, size.w, size.h, flags);
    mID = SDL_GetWindowID(Get());
}

std::uint32_t Window::GetID() const {
    return mID;
}

#endif
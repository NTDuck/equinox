#ifndef SERVICES_WINDOW_CPP
#define SERVICES_WINDOW_CPP

#include <services.hpp>


void Window::Initialize(std::string_view const& title, Rect const& size, std::uint32_t flags) {
    utility::SmartPointer<SDL_Window, SDL_CreateWindow, SDL_DestroyWindow>::Reset(title.data(), size.x, size.y, size.w, size.h, flags);
    mID = SDL_GetWindowID(Get());
}

std::uint32_t Window::GetID() const {
    return mID;
}


#endif
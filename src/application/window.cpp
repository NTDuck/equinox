#ifndef APPLICATION_WINDOW_CPP
#define APPLICATION_WINDOW_CPP

#include <application.hpp>


void Application::Window::Initialize(std::string_view const& title, Rect const& size, std::uint32_t flags) {
    Application::Dependency<SDL_Window, decltype(SDL_CreateWindow), decltype(SDL_DestroyWindow)>::Initialize<SDL_CreateWindow, SDL_DestroyWindow>(title.data(), size.x, size.y, size.w, size.h, flags);
    mID = SDL_GetWindowID(Get());
}

std::uint32_t Application::Window::GetID() const {
    return mID;
}

#endif
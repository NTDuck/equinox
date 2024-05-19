#ifndef APPLICATION_RENDERER_CPP
#define APPLICATION_RENDERER_CPP

#include <application.hpp>


void Application::Renderer::Clear() const {
    SDL_RenderClear(Get());
}

void Application::Renderer::Integrate() const {
    SDL_RenderPresent(Get());
}

void Application::Renderer::SetDrawColor(Color const& color) const {
    SDL_SetRenderDrawColor(Get(), color.r, color.g, color.b, color.a);
}

void Application::Renderer::FillRect(Rect const& rect) const {
    SDL_RenderFillRect(Get(), &rect);
}

void Application::Renderer::FillRect() const {
    SDL_RenderFillRect(Get(), nullptr);
}


#endif
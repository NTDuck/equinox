#ifndef APPLICATION_CPP
#define APPLICATION_CPP

#include <iostream>
#include <application.hpp>
#include <auxiliaries.hpp>


void Application::Start() {
    Initialize();
    GameLoop();
}

Application::~Application() {
    IMG_Quit();
    SDL_Quit();
}

void Application::Initialize() {
    SDL_Init(config::sdl::kInitFlags);
    IMG_Init(config::sdl::kInitFlagsImage);

    for (auto const& [hintName, hintValue] : config::sdl::kHints)
        SDL_SetHint(hintName.data(), hintValue.data());
}

void Application::GameLoop() {
    std::cout << "Hello world" << std::endl;
}


#endif
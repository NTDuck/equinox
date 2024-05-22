#ifndef APPLICATION_CPP
#define APPLICATION_CPP

#include <iostream>

#include <application.hpp>
#include <components.hpp>
#include <utilities.hpp>


Application::Application() : mCoordinator(std::make_shared<ecs::Coordinator>()) {}

Application::~Application() {
    IMG_Quit();
    SDL_Quit();
}

void Application::Start() {
    Initialize();
    StartGameLoop();
}

void Application::Initialize() {
    InitializeDependencies();
    RegisterComponents();
    RegisterSystems();
    CreateEntities();
}

/**
 * @warning Spaghetti code, sad to witness.
*/
void Application::StartGameLoop() {
    mRenderer.SetDrawColor({ 0xf2, 0xf3, 0xf4, SDL_ALPHA_OPAQUE });

    std::uint64_t dt = 0;
    SDL_Event event;
    bool flag = true;

    while (flag) {
        mFPSRegulator.PreIntegrate();

        while (SDL_PollEvent(&event)) if (event.type == SDL_QUIT) { flag = false; break; }

        mRenderer.Clear();

        mMovementSystem->Integrate(dt);
        mRenderSystem->Integrate(dt);

        // // Rudimentary logging to show that player actually moves
        // auto playerTransform = mCoordinator->GetComponent<components::Transform>(mPlayerID);
        // if (playerTransform.position.x != config::kMapHigherBound.x) std::cout << "Player position: " << playerTransform.position << std::endl;

        // std::cout << "FPS: " << mFPSMonitor.GetFPS(dt) << std::endl;

        mRenderer.RenderPresent();

        dt = mTimer.GetDeltaTime();
        mFPSRegulator.PostIntegrate();
    }
}

void Application::InitializeDependencies() {
    SDL_Init(config::sdl::kInitFlags);
    IMG_Init(config::sdl::kInitFlagsImage);

    for (auto const& pair : config::sdl::kHints) SDL_SetHint(pair.first.data(), pair.second.data());

    mWindow.Initialize(config::sdl::window::kTitle, config::sdl::window::kSize, config::sdl::window::kInitFlags);
    mRenderer.Initialize(mWindow, config::sdl::renderer::kDriverIndex, config::sdl::renderer::kInitFlags);

    mTimer.Start();
    mFPSMonitor.Start();
    mFPSRegulator.SetFPS(config::kFPS);
}

void Application::RegisterComponents() const {
    mCoordinator->RegisterComponents<components::Transform, components::Motion, components::Sprite>();
}

void Application::RegisterSystems() {
    mMovementSystem = mCoordinator->RegisterSystem<MovementSystem>(mCoordinator);
    mCoordinator->SetSystemSignature<MovementSystem, components::Transform, components::Motion>();

    mRenderSystem = mCoordinator->RegisterSystem<RenderSystem>(mCoordinator, mRenderer);
    mCoordinator->SetSystemSignature<RenderSystem, components::Transform, components::Sprite>();

    mRenderSystem->Initialize(config::kSpriteSheetData);
}

void Application::CreateEntities() {
    mPlayerID = mCoordinator->CreateEntity();

    mCoordinator->InsertComponent<components::Transform>(mPlayerID, {
        config::sdl::window::kSize.w >> 1,
        config::sdl::window::kSize.h >> 1,
    });
    mCoordinator->InsertComponent<components::Motion>(mPlayerID, {
        { 1, 1 }, { 0, 0 },
    });
    mCoordinator->InsertComponent<components::Sprite>(mPlayerID, {
        0, 20,
    });
}


#endif
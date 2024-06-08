#ifndef APPLICATION_CPP
#define APPLICATION_CPP

#include <application.hpp>
#include <systems.hpp>
#include <components.hpp>
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
    InitializeServices();
    RegisterComponents();
    RegisterSystems();
}

void Application::InitializeServices() {
    SDL_Init(config::sdl::kInitFlags);
    IMG_Init(config::sdl::kInitFlagsImage);

    for (auto const& [hintName, hintValue] : config::sdl::kHints)
        SDL_SetHint(hintName.data(), hintValue.data());

    mCoordinator = std::make_shared<ecs::Coordinator>();

    mWindow.Initialize(config::sdl::window::kTitle, config::sdl::window::kSize, config::sdl::window::kInitFlags);
    mRenderer.Initialize(mWindow, config::sdl::renderer::kDriverIndex, config::sdl::renderer::kInitFlags);

    mTimer.Start();
    mFPSMonitor.Start();
    mFPSRegulator.SetFPS(config::kFPS);
}

void Application::RegisterComponents() {
    mCoordinator->RegisterComponent<components::Transform>();
    mCoordinator->RegisterComponent<components::Motion>();
    mCoordinator->RegisterComponent<components::Sprite>();
}

void Application::RegisterSystems() {
    mMovementSystem = mCoordinator->RegisterSystem<systems::Movement>(mCoordinator);
    mCoordinator->SetSystemSignature<systems::Movement, components::Transform, components::Motion>();

    mRenderSystem = mCoordinator->RegisterSystem<systems::Render>(mCoordinator, mRenderer);
    mCoordinator->SetSystemSignature<systems::Render, components::Transform, components::Sprite>();
    mRenderSystem->Initialize(config::kSpriteSheetData);

    mArbitraryLoopSystem = mCoordinator->RegisterSystem<systems::ArbitraryLoop>(mCoordinator);
    mCoordinator->SubscribeEvent<&systems::ArbitraryLoop::OnArbitraryEvent>(mArbitraryLoopSystem);
}

void Application::GameLoop() {
    mRenderer.SetDrawColor(utility::GetColor{}("#f2f3f4"));

    std::uint64_t dt = 0;
    bool flag = true;
    SDL_Event event;

    auto playerID = mCoordinator->CreateEntity();

    mCoordinator->AddComponent<components::Transform>(playerID, {
        { config::sdl::window::kSize.w >> 1,
        config::sdl::window::kSize.h >> 1, },
        0,
    });
    mCoordinator->AddComponent<components::Motion>(playerID, {
        { 1, 1 }, { 0, 0 },
    });
    mCoordinator->AddComponent<components::Sprite>(playerID, {
        0, 20,
    });

    while (flag) {
        mFPSRegulator.PreIntegrate();

        while (SDL_PollEvent(&event)) if (event.type == SDL_QUIT) { flag = false; break; }

        mRenderer.Clear();

        mMovementSystem->Integrate(dt);
        mRenderSystem->Integrate();

        mRenderer.RenderPresent();

        dt = mTimer.GetDeltaTime();
        
        mFPSRegulator.PostIntegrate();
    }
}


#endif
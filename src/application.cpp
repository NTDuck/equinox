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

    mEventManager = std::make_unique<EventManager>(mCoordinator);
    mEventManager->SubscribeEventType(SDL_QUIT);
    mEventManager->SubscribeEventType(SDL_KEYDOWN);
    mEventManager->SubscribeEventType(SDL_KEYUP);

    mTimer.Start();
    mFPSMonitor.Start();
    mFPSRegulator.SetFPS(config::kFPS);
}

void Application::RegisterComponents() {
    mCoordinator->RegisterComponent<components::Transform>();
    mCoordinator->RegisterComponent<components::Motion>();
    mCoordinator->RegisterComponent<components::Sprite>();
    mCoordinator->RegisterComponent<components::PlayerInput>();
}

void Application::RegisterSystems() {
    mMovementSystem = mCoordinator->RegisterSystem<systems::Movement>(mCoordinator);
    mCoordinator->SetSystemSignature<systems::Movement, components::Transform, components::Motion>();

    mRenderSystem = mCoordinator->RegisterSystem<systems::Render>(mCoordinator, mRenderer);
    mCoordinator->SetSystemSignature<systems::Render, components::Transform, components::Sprite>();
    mRenderSystem->Initialize(config::kSpriteSheetData);

    mPlayerInputSystem = mCoordinator->RegisterSystem<systems::PlayerInput>(mCoordinator);
    mCoordinator->SubscribeEvent<&systems::PlayerInput::OnKeyboardEvent>(mPlayerInputSystem);

    mArbitraryLoopSystem = mCoordinator->RegisterSystem<systems::ArbitraryLoop>(mCoordinator);
    mCoordinator->SubscribeEvent<&systems::ArbitraryLoop::OnArbitraryEvent>(mArbitraryLoopSystem);

    mArbitraryTerminator = mCoordinator->RegisterSystem<systems::ArbitraryApplicationTerminator>();
    mCoordinator->SubscribeEvent<&systems::ArbitraryApplicationTerminator::Integrate>(mArbitraryTerminator);
}

void Application::GameLoop() {
    mRenderer.SetDrawColor(utility::GetColor{}("#f2f3f4"));

    double dt = 0;

    auto playerID = mCoordinator->CreateEntity();
    mCoordinator->AddComponent<components::Transform>(playerID, {
        { config::sdl::window::kSize.w >> 1,
        config::sdl::window::kSize.h >> 1, },
        0,
    });
    mCoordinator->AddComponent<components::Motion>(playerID, {
        { 0, 0 }, { 4, 4 }, { 0, 0 },
    });
    mCoordinator->AddComponent<components::Sprite>(playerID, {
        0, 20,
    });
    mCoordinator->AddComponent<components::PlayerInput>(playerID, {
        true,
    });

    while (mArbitraryTerminator->GetStatus()) {
        mFPSRegulator.PreIntegrate();
        mEventManager->Integrate();
        mRenderer.Clear();

        mMovementSystem->Integrate(dt);
        mRenderSystem->Integrate();

        mRenderer.RenderPresent();
        dt = mTimer.GetDeltaTime() / 10;
        mFPSRegulator.PostIntegrate();
    }
}


#endif
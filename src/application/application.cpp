#ifndef APPLICATION_CPP
#define APPLICATION_CPP

#include <fstream>
#include <iomanip>

#include <application.hpp>
#include <components.hpp>


Application::Application() : mCoordinator(std::make_shared<ecs::Coordinator>()) {}

Application::~Application() {
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
        while (SDL_PollEvent(&event)) if (event.type == SDL_QUIT) { flag = false; break; }

        mFPSRegulator.PreIntegrate();

        mRenderer.Clear();

        mMovementSystem->Integrate(dt);

        // Rudimentary logging to show that player actually moves
        auto playerTransform = mCoordinator->GetComponent<component::Transform>(mPlayerID);
        if (playerTransform.position.x != config::kMapHigherBound.x) std::cout << "Player position: (" << playerTransform.position.x << ", " << playerTransform.position.y << ")\n";
        std::cout << "FPS: " << mFPSMonitor.GetFPS(dt) << std::endl;

        mRenderer.FillRect();
        mRenderer.Integrate();

        dt = mTimer.GetDeltaTime();
        mFPSRegulator.PostIntegrate();
    }
}

void Application::InitializeDependencies() {
    SDL_Init(config::sdl::kInitFlags);
    for (auto const& pair : config::sdl::kHints) SDL_SetHint(pair.first.data(), pair.second.data());

    mWindow.Initialize(config::sdl::window::kTitle, config::sdl::window::kSize, config::sdl::window::kInitFlags);
    mRenderer.Initialize(mWindow, config::sdl::renderer::kDriverIndex, config::sdl::renderer::kInitFlags);

    mTimer.Start();
    mFPSMonitor.Start();
    mFPSRegulator.SetFPS(config::kFPS);
}

void Application::RegisterComponents() const {
    mCoordinator->RegisterComponent<component::Transform, component::Motion>();
}

void Application::RegisterSystems() {
    mMovementSystem = mCoordinator->RegisterSystem<MovementSystem>(mCoordinator);
    mCoordinator->SetSystemSignature<MovementSystem, component::Transform, component::Motion>();
}

void Application::CreateEntities() {
    mPlayerID = mCoordinator->CreateEntity();
    mCoordinator->InsertComponent<component::Transform>(mPlayerID, {
        { (config::kMapHigherBound.x - config::kMapLowerBound.x) / 2, (config::kMapHigherBound.y - config::kMapLowerBound.y) / 2 },
    });
    mCoordinator->InsertComponent<component::Motion>(mPlayerID, {
        { 1, 1 }, { 0, 0 },
    });
}


#endif
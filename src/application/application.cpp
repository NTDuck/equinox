#ifndef APPLICATION_CPP
#define APPLICATION_CPP

#include <fstream>
#include <iomanip>

#include <application.hpp>
#include <components.hpp>


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
        auto playerTransform = global::ECSCoordinator.GetComponent<component::Transform>(mPlayerID);
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
    global::ECSCoordinator.RegisterComponent<component::Transform>();
    global::ECSCoordinator.RegisterComponent<component::Motion>();
}

void Application::RegisterSystems() {
    mMovementSystem = global::ECSCoordinator.RegisterSystem<MovementSystem>();
    ECS::Signature movementSystemSignature;

    movementSystemSignature.set(global::ECSCoordinator.GetComponentID<component::Transform>());
    movementSystemSignature.set(global::ECSCoordinator.GetComponentID<component::Motion>());

    global::ECSCoordinator.SetSystemSignature<MovementSystem>(movementSystemSignature);
}

void Application::CreateEntities() {
    mPlayerID = global::ECSCoordinator.CreateEntity();

    component::Transform playerTransform;
    playerTransform.position.x = (config::kMapHigherBound.x - config::kMapLowerBound.x) / 2;
    playerTransform.position.y = (config::kMapHigherBound.y - config::kMapLowerBound.y) / 2;
    global::ECSCoordinator.InsertComponent(mPlayerID, playerTransform);

    component::Motion playerMotion;
    playerMotion.velocity.x = 1;
    playerMotion.velocity.y = 1;
    playerMotion.acceleration.x = 0;
    playerMotion.acceleration.y = 0;
    global::ECSCoordinator.InsertComponent(mPlayerID, playerMotion);
}


#endif
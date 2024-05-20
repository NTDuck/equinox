#ifndef APPLICATION_CPP
#define APPLICATION_CPP

#include <application.hpp>

#include <components.hpp>
#include <chrono>


void Application::Start() {
    Initialize();
    StartGameLoop();
}

void Application::Initialize() {
    InitializeExternalDependencies();
    RegisterComponents();
    RegisterSystems();
    CreateEntities();
}

void Application::StartGameLoop() const {
    std::pair<std::chrono::steady_clock::time_point, std::chrono::steady_clock::time_point> frameTimePoint;
    std::uint32_t dt = 0;

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    mRenderer.SetDrawColor({ 0xf2, 0xf3, 0xf4, SDL_ALPHA_OPAQUE });

    while (true) {
        mRenderer.Clear();

        frameTimePoint.first = std::chrono::steady_clock::now();

        mMovementSystem->Integrate(dt);
        
        frameTimePoint.second = std::chrono::steady_clock::now();
        dt = 1 + std::chrono::duration_cast<std::chrono::milliseconds>(frameTimePoint.second - frameTimePoint.first).count();

        // Rudimentary logging to show that player actually moves
        auto playerTransform = global::ECSCoordinator.GetComponent<component::Transform>(mPlayerID);
        std::cout << "(" << playerTransform.position.x << ", " << playerTransform.position.y << ")\n";
        if (playerTransform.position.x == config::kMapHigherBound.x) break;

        mRenderer.FillRect();
        mRenderer.Integrate();
    }
}

void Application::InitializeExternalDependencies() {
    SDL_Init(config::sdl::kInitFlags);
    for (auto const& pair : config::sdl::kHints) SDL_SetHint(pair.first.data(), pair.second.data());

    mWindow.Initialize(config::sdl::window::kTitle, config::sdl::window::kSize, config::sdl::window::kInitFlags);
    mRenderer.Initialize(mWindow, config::sdl::renderer::kDriverIndex, config::sdl::renderer::kInitFlags);
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
#ifndef MASTER_COORDINATOR_CPP
#define MASTER_COORDINATOR_CPP

#include <coordinators.hpp>
#include <chrono>


void MasterCoordinator::Start() {
    Initialize();
    GameLoop();
}

void MasterCoordinator::Initialize() {
    RegisterComponents();
    RegisterSystems();
    CreateEntities();
}

void MasterCoordinator::GameLoop() const {
    std::pair<std::chrono::steady_clock::time_point, std::chrono::steady_clock::time_point> frameTimePoint;
    std::uint32_t dt = 0;

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    while (true) {
        frameTimePoint.first = std::chrono::steady_clock::now();

        mMovementSystem->Integrate(dt);
        
        frameTimePoint.second = std::chrono::steady_clock::now();
        dt = 1 + std::chrono::duration_cast<std::chrono::milliseconds>(frameTimePoint.second - frameTimePoint.first).count();

        // Rudimentary logging to show that player actually moves
        auto playerTransform = global::coordinator.GetComponent<components::Transform>(mPlayerID);
        std::cout << "(" << playerTransform.position.x << ", " << playerTransform.position.y << ")\n";
        if (playerTransform.position.x == config::kMapHigherBound.x) break;
    }
}

void MasterCoordinator::RegisterComponents() const {
    global::coordinator.RegisterComponent<components::Transform>();
    global::coordinator.RegisterComponent<components::Motion>();
}

void MasterCoordinator::RegisterSystems() {
    mMovementSystem = global::coordinator.RegisterSystem<MovementSystem>();
    type::Signature movementSystemSignature;

    movementSystemSignature.set(global::coordinator.GetComponentID<components::Transform>());
    movementSystemSignature.set(global::coordinator.GetComponentID<components::Motion>());

    global::coordinator.SetSystemSignature<MovementSystem>(movementSystemSignature);
}

void MasterCoordinator::CreateEntities() {
    mPlayerID = global::coordinator.CreateEntity();

    components::Transform playerTransform;
    playerTransform.position.x = (config::kMapHigherBound.x - config::kMapLowerBound.x) / 2;
    playerTransform.position.y = (config::kMapHigherBound.y - config::kMapLowerBound.y) / 2;
    global::coordinator.InsertComponent(mPlayerID, playerTransform);

    components::Motion playerMotion;
    playerMotion.velocity.x = 1;
    playerMotion.velocity.y = 1;
    playerMotion.acceleration.x = 0;
    playerMotion.acceleration.y = 0;
    global::coordinator.InsertComponent(mPlayerID, playerMotion);
}


#endif
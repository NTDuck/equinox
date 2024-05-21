#ifndef SYSTEM_MOVEMENT_CPP
#define SYSTEM_MOVEMENT_CPP

#include <algorithm>

#include <components.hpp>
#include <systems.hpp>


MovementSystem::MovementSystem(std::shared_ptr<ecs::Coordinator> coordinator) : ecs::ISystem(coordinator) {}

void MovementSystem::Integrate(std::uint32_t dt) {
    for (auto const& entityID : mEntityIDs) {
        auto& transform = mCoordinator->GetComponent<component::Transform>(entityID);
        auto& motion = mCoordinator->GetComponent<component::Motion>(entityID);

        transform.position.x += motion.velocity.x * dt;
        transform.position.y += motion.velocity.y * dt;

        transform.position.x = std::clamp(transform.position.x, config::kMapLowerBound.x, config::kMapHigherBound.x);
        transform.position.y = std::clamp(transform.position.y, config::kMapLowerBound.y, config::kMapHigherBound.y);

        motion.velocity.x += motion.acceleration.x * dt;
        motion.velocity.y += motion.acceleration.y * dt;
    }
}


#endif
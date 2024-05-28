#ifndef SYSTEMS_MOVEMENT_CPP
#define SYSTEMS_MOVEMENT_CPP

#include <algorithm>

#include <components.hpp>
#include <systems.hpp>


systems::Movement::Movement(std::shared_ptr<ecs::Coordinator> coordinator) : ecs::ISystem(coordinator) {}

void systems::Movement::Integrate(std::uint32_t dt) const {
    for (auto const& entityID : mEntityIDs) {
        auto& transform = mCoordinator->GetComponent<components::Transform>(entityID);
        auto& motion = mCoordinator->GetComponent<components::Motion>(entityID);

        transform.position.x += motion.velocity.x * dt;
        transform.position.y += motion.velocity.y * dt;

        transform.position.x = std::clamp(transform.position.x, 0, config::sdl::window::kSize.w / 4 * 3);
        transform.position.y = std::clamp(transform.position.y, 0, config::sdl::window::kSize.h / 4 * 3);

        motion.velocity.x += motion.acceleration.x * dt;
        motion.velocity.y += motion.acceleration.y * dt;
    }
}


#endif
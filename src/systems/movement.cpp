#ifndef SYSTEMS_MOVEMENT_CPP
#define SYSTEMS_MOVEMENT_CPP

#include <algorithm>

#include <components.hpp>
#include <systems.hpp>


systems::Movement::Movement(std::shared_ptr<ecs::Coordinator> coordinator) : ecs::ISystem(coordinator) {}

void systems::Movement::Integrate(std::uint32_t dt) const {
    for (auto const& entityID : mEntityIDs) {
        auto& position = mCoordinator->GetMember<Member::position>(entityID);
        auto& velocity = mCoordinator->GetMember<Member::velocity>(entityID);
        auto& acceleration = mCoordinator->GetMember<Member::acceleration>(entityID);

        position.x += velocity.x * dt;
        position.y += velocity.y * dt;

        velocity.x += acceleration.x * dt;
        velocity.y += acceleration.y * dt;

        position.x = std::clamp(position.x, 0, config::sdl::window::kSize.w / 4 * 3);
        position.y = std::clamp(position.y, 0, config::sdl::window::kSize.h / 4 * 3);
    }
}


#endif
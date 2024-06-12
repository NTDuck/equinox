#ifndef SYSTEMS_MOVEMENT_CPP
#define SYSTEMS_MOVEMENT_CPP

#include <components.hpp>
#include <systems.hpp>


systems::Movement::Movement(std::shared_ptr<ecs::Coordinator> coordinator) : ecs::ISystem(coordinator) {}

void systems::Movement::Integrate(double dt) const {
    using namespace components;

    for (auto const& entityID : mEntityIDs) {
        auto& position = mCoordinator->GetMember<Transform, Transform::kPosition>(entityID);
        auto& velocity = mCoordinator->GetMember<Motion, Motion::kVelocity>(entityID);
        auto const& maxVelocity = mCoordinator->GetMember<Motion, Motion::kMaxVelocity>(entityID);
        auto& acceleration = mCoordinator->GetMember<Motion, Motion::kAcceleration>(entityID);

        velocity.x = std::clamp(velocity.x, -maxVelocity.x, maxVelocity.x);
        velocity.y = std::clamp(velocity.y, -maxVelocity.y, maxVelocity.y);

        velocity.x += acceleration.x * dt;
        velocity.y += acceleration.y * dt;

        position.x += velocity.x * dt;
        position.y += velocity.y * dt;

        position.x = std::clamp(position.x, 0, config::sdl::window::kSize.w);
        position.y = std::clamp(position.y, 0, config::sdl::window::kSize.h);

        if (position.x >= config::sdl::window::kSize.w || position.y >= config::sdl::window::kSize.h)
            mCoordinator->PublishEvent(std::make_shared<events::ArbitraryEvent>(entityID));
    }
}


#endif
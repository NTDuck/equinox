#ifndef SYSTEMS_PLAYER_INPUT_CPP
#define SYSTEMS_PLAYER_INPUT_CPP

#include <components.hpp>
#include <systems.hpp>


systems::PlayerInput::PlayerInput(std::shared_ptr<ecs::Coordinator> coordinator) : ecs::ISystem(coordinator) {}

void systems::PlayerInput::OnKeyboardEvent(std::shared_ptr<EventManager::Event<SDL_KeyboardEvent>> event) const {
    using namespace components;

    if (event->repeat) return;

    for (auto const& entityID : mEntityIDs) {
        auto const& enable = mCoordinator->GetMember<components::PlayerInput, components::PlayerInput::kEnable>(entityID);
        if (!enable) continue;

        auto& acceleration = mCoordinator->GetMember<Motion, Motion::kAcceleration>(entityID);

        switch (event->keysym.sym) {
        case config::key::kPlayerMoveUp:
            acceleration.y = event->type == SDL_KEYDOWN ? -1 : 0;
            break;

        case config::key::kPlayerMoveDown:
            acceleration.y = event->type == SDL_KEYDOWN ? 1 : 0;
            break;

        case config::key::kPlayerMoveLeft:
            acceleration.x = event->type == SDL_KEYDOWN ? -1 : 0;
            break;

        case config::key::kPlayerMoveRight:
            acceleration.x = event->type == SDL_KEYDOWN ? 1 : 0;
            break;

        default:
            break;
        }
    }
}


#endif
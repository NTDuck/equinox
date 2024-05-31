#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <ecs.hpp>
#include <auxiliaries.hpp>


enum class ecs::ext::ComponentMember {
    position, rotation,   // Transform
    velocity, acceleration,   // Motion
    spriteSheetID, spriteID,   // Sprite
};

/**
 * @see https://docs.unity3d.com/ScriptReference/
*/
namespace components {
    struct Transform : public ecs::IComponent<Point, double> {};
    struct Motion : public ecs::IComponent<FPoint, FPoint> {};
    struct Sprite : public ecs::IComponent<SpriteSheetID, SpriteID> {};

    struct Collider;

    struct Health;
    struct Follow;
    struct Joystick;
}

class ecs::ext::ComponentMap : public ecs::internal::ComponentMap<
    ecs::internal::ComponentMapData<Member::position, components::Transform, 0>,
    ecs::internal::ComponentMapData<Member::rotation, components::Transform, 1>,
    ecs::internal::ComponentMapData<Member::velocity, components::Motion, 0>,
    ecs::internal::ComponentMapData<Member::acceleration, components::Motion, 1>,
    ecs::internal::ComponentMapData<Member::spriteSheetID, components::Sprite, 0>,
    ecs::internal::ComponentMapData<Member::spriteID, components::Sprite, 1>
> {};


#endif
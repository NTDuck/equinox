#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <ecs.hpp>
#include <auxiliaries.hpp>


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

enum class ecs::ext::ComponentMember {
    kPosition, kRotation,   // Transform
    kVelocity, kAcceleration,   // Motion
    kSpriteSheetID, kSpriteID,   // Sprite
};

class ecs::ext::ComponentMap : public ecs::internal::ComponentMap<
    ecs::internal::ComponentMapData<Member::kPosition, components::Transform, 0>,
    ecs::internal::ComponentMapData<Member::kRotation, components::Transform, 1>,
    ecs::internal::ComponentMapData<Member::kVelocity, components::Motion, 0>,
    ecs::internal::ComponentMapData<Member::kAcceleration, components::Motion, 1>,
    ecs::internal::ComponentMapData<Member::kSpriteSheetID, components::Sprite, 0>,
    ecs::internal::ComponentMapData<Member::kSpriteID, components::Sprite, 1>
> {};


#endif
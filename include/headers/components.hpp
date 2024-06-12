#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <ecs.hpp>
#include <auxiliaries.hpp>


/**
 * @see https://docs.unity3d.com/ScriptReference/
*/
namespace components {
    struct Transform : public ecs::IComponent<Point, double> {
        static constexpr std::size_t kPosition = 0;
        static constexpr std::size_t kRotation = 1;
    };

    struct Motion : public ecs::IComponent<FPoint, FPoint, FPoint> {
        static constexpr std::size_t kVelocity = 0;
        static constexpr std::size_t kMaxVelocity = 1;
        static constexpr std::size_t kAcceleration = 2;
    };

    struct Sprite : public ecs::IComponent<SpriteSheetID, SpriteID> {
        static constexpr std::size_t kSpriteSheetID = 0;
        static constexpr std::size_t kSpriteID = 1;
    };

    struct PlayerInput : public ecs::IComponent<bool> {
        static constexpr std::size_t kEnable = 0;
    };

    struct Collider;

    struct Health;
    struct Follow;
    struct Joystick;
}


#endif
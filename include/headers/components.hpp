#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <ecs.hpp>
#include <auxiliaries.hpp>


/**
 * @see https://docs.unity3d.com/ScriptReference/
*/
namespace components {
    /**
     * @param 0 position
     * @param 1 rotation
    */
    struct Transform : public ecs::IComponent<Point, double> {};

    /**
     * @param 0 velocity
     * @param 1 acceleration
    */
    struct Motion : public ecs::IComponent<FPoint, FPoint> {};

    /**
     * @param 0 spriteSheetID
     * @param 1 spriteID
    */
    struct Sprite : public ecs::IComponent<SpriteSheetID, SpriteID> {};

    struct Collider;

    struct Health;
    struct Follow;
    struct Joystick;
}


#endif
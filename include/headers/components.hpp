#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <auxiliaries.hpp>


/**
 * @see https://medium.com/@savas/nomad-game-engine-part-2-ecs-9132829188e5
*/
namespace component {
    struct Transform {
        Point position;
    };

    struct Motion {
        Point velocity;
        Point acceleration;
    };

    struct Sprite {
        SpriteSheetID spriteSheetID;
        SpriteID spriteID;
    };

    struct Collision;
    struct Health;
    struct Follow;
    struct Joystick;
}


#endif
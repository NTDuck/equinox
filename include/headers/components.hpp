#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <auxiliaries.hpp>


/**
 * @note Default member initializers make the type non-trivial therefore non-POD.
 * @see https://docs.unity3d.com/ScriptReference/
*/
namespace components {
    struct Transform {
        Point position;
        double rotation;
    };

    struct Motion {

    };

    struct Sprite;
    struct Collider;

    struct Health;
    struct Follow;
    struct Joystick;
}


#endif
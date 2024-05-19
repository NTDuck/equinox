#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <auxiliaries.hpp>


namespace component {
    struct Transform {
        Point position;
    };

    struct Motion {
        Point velocity;
        Point acceleration;
    };
}


#endif
#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <ecs.hpp>


namespace events {
    struct ArbitraryEvent : ecs::IEvent {
        inline ArbitraryEvent(ecs::EntityID entityID) : entityID(entityID) {}

        ecs::EntityID entityID;
    };
}


#endif
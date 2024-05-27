#ifndef ECS_COMPONENT_MANAGER_CPP
#define ECS_COMPONENT_MANAGER_CPP

#include <ecs.hpp>


void ecs::ComponentManager::EntityDestroyedCallback(EntityID entityID) {
    for (auto const& [componentTypeName, component] : mComponentArrays)
        component->EntityDestroyedCallback(entityID);
}


#endif
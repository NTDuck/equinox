#ifndef ECS_COMPONENT_ARRAY_TPP
#define ECS_COMPONENT_ARRAY_TPP

#include <ecs.hpp>


template <typename Component>
void ecs::ComponentArray<Component>::EntityDestroyedCallback(EntityID entityID) {
    if (this->mObjectIDToMemberIndexMap.find(entityID) != this->mObjectIDToMemberIndexMap.end())
        this->Erase(entityID);
}


#endif
#ifndef ECS_COMPONENT_ARRAY_TPP
#define ECS_COMPONENT_ARRAY_TPP

#include <ecs.hpp>


template <typename Component>
void ecs::ComponentArray<Component>::EntityDestroyedCallback(EntityID entityID) {
    if (utility::StructOfArray<EntityID, config::kMaxEntityID, typename Component::Object>::mObjectIDToMemberIndexMap.find(entityID) != utility::StructOfArray<EntityID, config::kMaxEntityID, typename Component::Object>::mObjectIDToMemberIndexMap.end())
        utility::StructOfArray<EntityID, config::kMaxEntityID, typename Component::Object>::Erase(entityID);
}


#endif
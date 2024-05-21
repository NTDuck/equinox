#ifndef ECS_COMPONENT_ARRAY_TPP
#define ECS_COMPONENT_ARRAY_TPP

#include <cassert>
#include <ecs/components.hpp>


template <typename Component>
void ecs::ComponentArray<Component>::InsertComponent(EntityID entityID, Component const& component) {
    assert(mEntityToIndexMap.find(entityID) == mEntityToIndexMap.end() && "Component added to entity more than once");

    // Place new entry at the end
    std::size_t newIndex = mSize;
    mComponentArray[newIndex] = component;

    // Update maps
    mEntityToIndexMap[entityID] = newIndex;
    mIndexToEntityMap[newIndex] = entityID;
    ++mSize;
}

template <typename Component>
void ecs::ComponentArray<Component>::EraseComponent(EntityID entityID) {
    assert(mEntityToIndexMap.find(entityID) != mEntityToIndexMap.end() && "Removing non-existent component");

    // Copy element at the end into deleted element's place to maintain density
    std::size_t indexOfRemovedEntity = mEntityToIndexMap[entityID];
    std::size_t indexOfLastElement = mSize - 1;
    mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

    // Update map to point to moved spot
    auto entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
    mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
    mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

    mEntityToIndexMap.erase(entityID);
    mIndexToEntityMap.erase(indexOfLastElement);

    --mSize;    
}

template <typename Component>
Component& ecs::ComponentArray<Component>::GetComponent(EntityID entityID) {
    assert(mEntityToIndexMap.find(entityID) != mEntityToIndexMap.end() && "Retrieving non-existent component.");
    return mComponentArray[mEntityToIndexMap[entityID]];
}

template <typename Component>
void ecs::ComponentArray<Component>::EraseEntityCallback(EntityID entityID) {
    if (mEntityToIndexMap.find(entityID) != mEntityToIndexMap.end()) EraseComponent(entityID);
}


#endif
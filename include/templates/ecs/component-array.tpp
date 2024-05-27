#ifndef ECS_COMPONENT_ARRAY_TPP
#define ECS_COMPONENT_ARRAY_TPP

#include <cassert>
#include <functional>

#include <ecs.hpp>


template <typename Component>
void ecs::ComponentArray<Component>::Insert(EntityID entityID, Component const& component) {
    assert(mEntityToIndexMap.find(entityID) == mEntityToIndexMap.end() && "Component added to same entity more than once");

    // Place new element at the end
    auto indexOfNewElement = mSize++;

    // Update maps
    mEntityToIndexMap[entityID] = indexOfNewElement;
    mIndexToEntityMap[indexOfNewElement] = entityID;
    mComponentArray[indexOfNewElement] = component;
}

template <typename Component>
void ecs::ComponentArray<Component>::Erase(EntityID entityID) {
    assert(mEntityToIndexMap.find(entityID) != mEntityToIndexMap.end() && "Removing non-existent component");

    // Copy last element (back) into removed element's place
    auto indexOfRemovedEntity = mEntityToIndexMap[entityID];
    auto indexOfLastElement = --mSize;
    mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

    // Update maps
    auto entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
    mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
    mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

    mEntityToIndexMap.erase(entityID);
    mIndexToEntityMap.erase(indexOfLastElement);
}

template <typename Component>
Component& ecs::ComponentArray<Component>::Get(EntityID entityID) {
    assert(mEntityToIndexMap.find(entityID) != mEntityToIndexMap.end() && "Retrieving non-existent component");
    return mComponentArray[mEntityToIndexMap[entityID]];
}

template <typename Component>
void ecs::ComponentArray<Component>::EntityDestroyedCallback(EntityID entityID) {
    if (mEntityToIndexMap.find(entityID) != mEntityToIndexMap.end())
        Erase(entityID);
}

template <typename Component>
template <typename... Args>
void ecs::ComponentArray<Component>::Emplace(EntityID entityID, Args&&... args) {
    Insert(entityID, Component(std::forward<Args>(args)...));
}


#endif